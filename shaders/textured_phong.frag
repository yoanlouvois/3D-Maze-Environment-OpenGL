#version 330 core

out vec4 out_color;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 frag_tex_coords;

uniform sampler2D diffuse_map;
uniform vec3 lightColor;

void main()
{
    vec3 texColor = texture(diffuse_map, frag_tex_coords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float distance = length(LightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.2 * distance + 0.08 * distance * distance);

    float ambientStrength = 0.38;
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.08;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = ambient + attenuation * (diffuse + specular);
    vec3 result = lighting * texColor;

    out_color = vec4(result, 1.0);
}