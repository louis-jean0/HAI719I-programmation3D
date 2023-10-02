#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader
out vec3 o_color;
uniform float scale = 1;
uniform vec3 translation;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
layout(location = 1) in vec3 colorValues_modelspace;
//------
layout(location = 2) in int indicesSommets_modelspace;

void main() {

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    gl_Position =  vec4(scale * vertexPosition_modelspace + translation,1);
    o_color = colorValues_modelspace;

    //Assigner la normale à la variable interpolée color
    //ajouter ici
}

