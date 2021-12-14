#version 330
//--- out_Color: 버텍스 세이더에서 입력받는 색상 값
//--- FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨. 

in vec4 out_Color; //--- 버텍스 세이더에게서 전달 받음
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor; //--- 색상 출력

uniform sampler2D outTexture; // 텍스처 인덱스: 유니폼 변수로 0번부터 설정

uniform vec3 lightPos; //--- 광원 위치
uniform vec3 lightColor; //--- 광원 색상

uniform vec3 viewPos; //--- 카메라 위치

uniform bool lightOn;
uniform bool textureOn;

uniform float alphaValue;

void main(void) 
{
	float ambientLight = 0.3;
	vec3 ambient = ambientLight * lightColor;

	vec3 result;

	if (lightOn) {
		vec3 lightDir = normalize(lightPos - FragPos);
		float diffuseLight = max(dot(Normal, lightDir), 0.0);
		vec3 diffuse = diffuseLight * lightColor;

		int shininess = 128;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, Normal);
		float specularLight = max(dot(viewDir, reflectDir), 0.0);
		specularLight = pow(specularLight, shininess);
		vec3 specular = specularLight * lightColor;
	
		result = (ambient + diffuse + specular) * vec3(out_Color);
		if(textureOn){
			FragColor = texture(outTexture, TexCoord) * vec4((ambient + diffuse + specular), 1.0);
		}
		else{
			FragColor = vec4 (result, out_Color.a * alphaValue);
		}
	}
	else {
		result = ambient * vec3(out_Color);
		if(textureOn){
			FragColor = texture(outTexture, TexCoord) * vec4(ambient, 1.0);
		}
		else{
			FragColor = vec4 (result, out_Color.a * alphaValue);
		}
	}
}