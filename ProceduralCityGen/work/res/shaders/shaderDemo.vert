//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Values to pass to the fragment shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec3 vLightDir;


void main() {

	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	float distn, dx,dy, dz;
	distn = length(vPosition - vec3(0,0,0));
	dx = vPosition.x;
	dy = vPosition.y;
	dz = vPosition.z;
	//vPosition.y = vPosition.y - distn/5;
	//vPosition.xz = vec2(distn/abs(dx),distn/abs(dz));

	vLightDir = normalize(vec3(gl_LightSource[0].position));
	vTextureCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position.y = gl_Position.y + vPosition.y;
//	gl_Position.xz =gl_Position.xz -  vPosition.xz;
}