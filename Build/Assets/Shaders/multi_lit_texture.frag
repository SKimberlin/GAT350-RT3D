#version 430

#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 tiling;
	vec2 offset;
} material;

uniform struct Light
{
	int type;
	vec3 position;
	float range;
	vec3 direction;
	vec3 color;
	float intensity;
	float innerAngle;
	float outerAngle;
} lights[30];

uniform vec3 ambientLight = vec3(0.2, 0.2, 0.2);
uniform int numLights = 3;

layout(binding = 0) uniform sampler2D tex;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular)
{
	// DIFFUSE
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - position);

	float spotIntensity = 1;
	if (light.type == SPOT)
	{
		float angle = acos(dot(light.direction, -lightDir));
		//if (angle > light.innerAngle) spotIntensity = 0;
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
	diffuse = material.diffuse * (light.color * intensity);

	// SPECULAR
	specular = vec3(0);
	if (intensity > 0)
	{
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * spotIntensity;
	}
}
/*
vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = ambientLight;

	// ATTENUATION
	float attenuation = 1;
	if (light.type != DIRECTIONAL)
	{
		float distanceSqr = dot(light.position - position, light.position - position);
		float rangeSqr = pow(light.range, 2);
		attenuation = pow(max(0, 1 - pow(distanceSqr / rangeSqr, 2)), 2);

	}

	return ambient + (diffuse + specular) * light.intensity * attenuation;
}
*/

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	// set ambient light
	ocolor = vec4(ambientLight, 1) * texcolor;
 
	// set lights
	for (int i = 0; i < numLights; i++)
	{
		vec3 diffuse;
		vec3 specular;
 
		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, fposition, lights[i].range);
 
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * texcolor) + vec4(specular, 1)) * lights[i].intensity * attenuation;
	}
}
