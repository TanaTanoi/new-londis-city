#version 120
varying vec3 position;
varying vec3 normal;

uniform sampler2D texture0;
varying vec2 vTextureCoord0;

void main()
{
		float shininess = 1;
		int MAX_LIGHTS = 3;
		vec3 ambientCombo = vec3(0,0,0);
		vec3 diffuseCombo = vec3(0,0,0);
		vec3 specularCombo = vec3(0,0,0);
		for(int i =0; i < MAX_LIGHTS;i++){
			vec3 lightPos =gl_LightSource[i].position.xyz;
			vec3 lambient =gl_LightSource[i].ambient.xyz;  //gl_LightSource[0]
	 		vec3 ldiffuse =gl_LightSource[i].diffuse.xyz;
	 		vec3 lspecular=gl_LightSource[i].specular.xyz;

	 		vec3 surf2light=normalize(lightPos-position);

			float dist=length(position-lightPos);   //distance from light-source to surface
			float att=1.0/(1.0+0.1*dist+0.01*dist*dist);    //attenuation (constant,linear,quadric)

	 		if(gl_LightSource[i].position.w > 0){///in the case of spotlight lights and point lights

	 			float diff = dot(normalize(gl_LightSource[i].spotDirection.xyz),surf2light);
	 			diff = acos(diff/length(gl_LightSource[i].spotDirection.xyz)/length(surf2light));
	 			if(!(diff>3.1-gl_LightSource[i].spotCutoff)){
	 				continue;//skip this light if this fragment is out the scope of the light
	 			}
	 		}else{								//in the case of directional lights
	 			att = min(1,max(dot(lightPos,-normal),0.0)*gl_LightSource[i].position.w);
	 			surf2light = -lightPos;
	 		}

	        vec3 norm=normalize(normal);
	        float dcont=max(0.0,dot(norm,surf2light));

			vec3 surf2view=normalize(-position);
	        vec3 reflection=reflect(-surf2light,norm);
			float scont=pow(max(0.0,dot(surf2view,reflection)),shininess);

			ambientCombo+= lambient*att;
			diffuseCombo+=dcont*ldiffuse*att;
			specularCombo+=scont*lspecular*att;

		}

		vec3 color = texture2D(texture0, vTextureCoord0).rgb;
        vec3 ambient=color*ambientCombo; //the ambient light


        vec3 diffuse=color*diffuseCombo;

        vec3 specular=specularCombo*color;

        gl_FragColor=vec4((ambient+diffuse+specular),1.0);  //<- don't forget the paranthesis (ambient+diffuse+specular)
}