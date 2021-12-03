#version 330
//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��. 

in vec3 out_Color; //--- ���ؽ� ���̴����Լ� ���� ����
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor; //--- ���� ���

uniform vec3 lightPos; //--- ���� ��ġ
uniform vec3 lightColor; //--- ���� ����

uniform vec3 viewPos; //--- ī�޶� ��ġ

uniform bool lightOn;

void main(void) 
{
	float ambientLight = 0.3;
	vec3 ambient = ambientLight * lightColor;

	vec3 result;

	if (lightOn) {
		// vec3 normalVector = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diffuseLight = max(dot(Normal, lightDir), 0.0);
		vec3 diffuse = diffuseLight * lightColor;

		int shininess = 128;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, Normal);
		float specularLight = max(dot(viewDir, reflectDir), 0.0);
		specularLight = pow(specularLight, shininess);
		vec3 specular = specularLight * lightColor;
	
		// vec3 result = (ambient + diffuse + specular) * out_Color;
		result = (ambient + diffuse + specular) * out_Color;
	}
	else {
		result = ambient * out_Color;
	}
	FragColor = vec4 (result, 1.0);
}