#version 330 core
//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1
//--- in_Normal: attribute index 2

layout (location = 0) in vec3 in_Position; // 위치 변수: attribute position 0
layout (location = 1) in vec3 in_Color; // 컬러 변수: attribute position 1
layout (location = 2) in vec3 in_Normal; // 노멀 변수: attribute position 2

out vec3 out_Color;	//--- 프래그먼트 세이더에게 전달
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model; //--- 모델링 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름"model"로 받아옴
uniform mat4 view; //--- 뷰잉 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름"view"로 받아옴
uniform mat4 projection; //--- 투영 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름"projection"로 받아옴
uniform vec3 model_color;

void main()
{
	gl_Position = projection * view * model * vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);

	if (model_color.x > -1 && model_color.y > -1 && model_color.z > -1) { out_Color = model_color; }
	else { out_Color = in_Color; }

	FragPos = vec3(model * vec4(in_Position, 1.0));

	vec4 modelNormal = model * vec4 (in_Normal, 1.0) -  model * vec4 (0.0, 0.0, 0.0, 1.0);

	Normal = vec3(modelNormal.x, modelNormal.y, modelNormal.z);
}