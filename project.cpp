/*
 * GRUPO
 *
 * Componentes:
 * 		Carlos Antunes
 * 		Gabriel
 * 		Priscilla Cândido
 *
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "RayTracer.hpp"
#include "utils.hpp"

//constantes, somente pra ajudar
Material *M_RED = new Material(Vec(1,0,0), Vec(1,0,0), Vec(1,0,0), 5, 0.3, 1.3, true, false, false, false, NULL);
Material *M_GREEN = new Material(Vec(0,1,0), Vec(0,1,0), Vec(0,1,0), 5, 0.3, 1.3, true, false, false, false, NULL);
Material *M_BLUE = new Material(Vec(0,0,1), Vec(0,0,1), Vec(0,0,1), 5, 0.3, 1.3, true, false, false, false, NULL);
Material *M_WHITE = new Material(Vec(1,1,1), Vec(1,1,1), Vec(1,1,1), 5, 0.3, 1.3, true, false, false, false, NULL);
Material *M_YELLOW = new Material(Vec(1,1,0), Vec(1,1,0), Vec(1,1,0), 5, 0.3, 1.3, true, false, false, false, NULL);
Material *M_DARKER_RED = new Material(Vec(0.7,0,0), Vec(0.7,0,0), Vec(0.7,0,0), 5, 0.3, 1.3, true, false, false, false, NULL);

/*PENDENCIAS
 * texura de topo ainda com cor fixa
 * texura do lego
 * espessura do lego
 * fazer opaca/espelhada/ transparene (cilindro)
 * fazer refração
 * */

void createSquare(Scene *scene, Transform *transform, Material *material, Vec v1, Vec v2, Vec v3, Vec v4){

    Triangle *triangule1 = new Triangle (new Vertex(v1), new Vertex(v2), new Vertex(v3), false, material);
    triangule1->setTransform(transform);
    scene->addSurface(triangule1);

    Triangle *triangule2 = new Triangle (new Vertex(v3), new Vertex(v4), new Vertex(v1), true, material);
    triangule2->setTransform(transform);
    scene->addSurface(triangule2);
}

void createCylinder(Scene *scene, Material *material, Transform *tCylinder){


    //Transform *tCylinder = new Transform(0,0,0 , 0,0,-10, 1,1,1, 1);

    Vec v1(0,0,1);
    Vec v2(0,1,1);
    Vec v3(1,1,1);
    Vec v4(1,0,1);


    Transform *tfront = new Transform(0,0,0 , 0,0,0, 1,1,1, 1);
    createSquare(scene, tfront, material,v1, v2, v3, v4);
    tCylinder->addChild(tfront);

    Transform *tback = new Transform(0,0,0 , 0,0,-1.55, 1,1,1, 1);
    createSquare(scene, tback, material,v1, v2, v3, v4);
    tCylinder->addChild(tback);

    Transform *tleftBack = new Transform(0.9,0,0 , -1.4,0,-0.39, 1,1,1, 1);
    createSquare(scene, tleftBack, material,v1, v2, v3, v4);
    tCylinder->addChild(tleftBack);

    Transform *tleftFront = new Transform(-0.9,0,0 , 0.15,0,-0.4, 1,1,1, 1);
    createSquare(scene, tleftFront, material,v1, v2, v3, v4);
    tCylinder->addChild(tleftFront);

    Transform *trightBack = new Transform(-0.9,0,0 , 1.78,0,-1.2, 1,1,1, 1);
    createSquare(scene, trightBack, material,v1, v2, v3, v4);
    tCylinder->addChild(trightBack);

    Transform *trightFront = new Transform(0.9,0,0 , 0.2,0,0.37, 1,1,1, 1);
    createSquare(scene, trightFront, material,v1, v2, v3, v4);
    tCylinder->addChild(trightFront);
}

void createWall(Scene *scene, Transform *transform, Material *material, bool createTop){

    Vec v1(-1,-1,1);
    Vec v2(-1,1,1);
    Vec v3(2,1,1);
    Vec v4(2,-1,1);

    createSquare(scene, transform, material, v1, v2, v3, v4);

    if(createTop){

        Vec v1tf(v1.x, v1.y + 2, v1.z);
        Vec v2tf(v2.x + 0.3, v2.y + 0.2, 0.8);
        Vec v3tf(v3.x - 0.3, v3.y + 0.2, 0.8);
        Vec v4tf(v4.x, v4.y + 2, v4.z);

        Material *mtop = new Material(Vec(0.7,0,0), Vec(0.7,0,0), Vec(0.7,0,0), 5, 0.3, 1.3, true, false, false, false, NULL);

        createSquare(scene, transform, mtop, v1tf, v2tf, v3tf, v4tf);
    }
}

/*no futuro juntar createSideWall e createWall e passar vertices como parametro*/
void createSideWall(Scene *scene, Transform *transform, Material *material, bool createTop){

    Vec v1(-1,-1,0);
    Vec v2(-1,1,0);
    Vec v3(-1,1,2);
    Vec v4(-1,-1,2);

    createSquare(scene, transform, material, v1, v2, v3, v4);

     if(createTop){

        Vec v1tf(v1.x, v1.y + 2, v1.z);
        Vec v2tf(v2.x + 0.2, v2.y + (v2.y *0.2), v2.z + 0.2);
        Vec v3tf(v3.x + 0.2, v3.y + (v3.y *0.2), v3.z - 0.2);
        Vec v4tf(v4.x, v4.y + 2, v4.z);

        Material *mtop = new Material(Vec(0.7,0,0), Vec(0.7,0,0), Vec(0.7,0,0), 5, 0.3, 1.3, true, false, false, false, NULL);
        createSquare(scene, transform, mtop, v1tf, v2tf, v3tf, v4tf);
    }

}

void createLego2x2(Scene *scene,Transform *tfLego, Material *material,bool createTop){

    Transform *tCyndr1 = new Transform(0,0,0 , -0.4,1.2,-0.4, 0.3,0.3,0.3, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr1);
    tfLego->addChild(tCyndr1);

    Transform *tCyndr2 = new Transform(0,0,0 , -0.4,1.2,0.4, 0.3,0.3,0.3, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr2);
    tfLego->addChild(tCyndr2);

    Transform *tCyndr3 = new Transform(0,0,0 , 1,1.2,-0.4, 0.3,0.3,0.3, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr3);
    tfLego->addChild(tCyndr3);

    Transform *tCyndr4 = new Transform(0,0,0 , 1,1.2,0.4, 0.3,0.3,0.3, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr4);
    tfLego->addChild(tCyndr4);


    Transform *tfTop = new Transform(0,1.55,0 , 0.03,2.2,-0.02, 0.82,0.8,1, 1);
    createWall(scene, tfTop, material, false);
    tfLego->addChild(tfTop);

    Transform *tfFront = new Transform(0,0,0 , 0,0,0, 1,1,1, 1);
    createWall(scene, tfFront, material, createTop);
    tfLego->addChild(tfFront);

    Transform *tfBack = new Transform(3.15,0,0 , 1,0,0, 1,1,1, 1);
    createWall(scene, tfBack, material, createTop);
    tfLego->addChild(tfBack);

    Transform *tfLeft = new Transform(0,0,0 , 0,0,-1, 1,1,1, 1);
    createSideWall(scene, tfLeft, material, createTop);
    tfLego->addChild(tfLeft);

    Transform *tfRight = new Transform(3.15,0,0 , 0.48,0,1.01, 1.5,1,1, 1);
    createSideWall(scene, tfRight, material, createTop);
    tfLego->addChild(tfRight);

}

void createLego4x1(Scene *scene,Transform *tfLego, Material *material,bool createTop){

    //cylinder
    Transform *tCyndr1 = new Transform(0,0,0 , -0.5,0.5,0.45, 0.2,0.2,0.2, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr1);
    tfLego->addChild(tCyndr1);

    Transform *tCyndr2 = new Transform(0,0,0 , 0.2,0.5,0.45, 0.2,0.2,0.2, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr2);
    tfLego->addChild(tCyndr2);

    Transform *tCyndr3 = new Transform(0,0,0 , 1,0.5,0.45, 0.2,0.2,0.2, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr3);
    tfLego->addChild(tCyndr3);

    Transform *tCyndr4 = new Transform(0,0,0 , 0.2,0.5,0.45, 0.2,0.2,0.2, 1);
    createCylinder(scene, M_DARKER_RED, tCyndr4);
    tfLego->addChild(tCyndr4);
    //end cylinder


    Transform *tfTop = new Transform(0,1.55,0 , -0.2,1.5,0.4, 1.2,0.4,1, 1);
    createWall(scene, tfTop, material, false);
    tfLego->addChild(tfTop);

    Transform *tfFront = new Transform(0,0,0 , -0.3,0,0, 1.4,0.5,1, 1);
    createWall(scene, tfFront, material, createTop);
    tfLego->addChild(tfFront);

    Transform *tfFront2 = new Transform(0,0,0 , -0.3,0,-0.2, 1.4,0.5,1, 1);
    createWall(scene, tfFront2, M_BLUE, createTop);
    tfLego->addChild(tfFront2);


    Transform *tfBack = new Transform(3.15,0,0 , 1,0,1, 1.4,0.5,1, 1);
    createWall(scene, tfBack, material, createTop);
    tfLego->addChild(tfBack);

    Transform *tfLeft = new Transform(0,0,0 , -1,0,0, 1,0.5,0.5, 1);
    createSideWall(scene, tfLeft, M_YELLOW, createTop);
    tfLego->addChild(tfLeft);

    Transform *tfRight = new Transform(3.15,0,0 , 0.8,0,1.01, 1.5,0.5,0.5, 1);
    createSideWall(scene, tfRight, material, createTop);
    tfLego->addChild(tfRight);

}

int main(int argc, char **){

    Camera *camera = new Camera(-1, 1, 1, -1, 2, 100, 100, 400, 400);
    Scene *scene = new Scene();

    //background
    CornellBox(scene, new Transform(0.0, 0.0, 0.0, 0, 0, -10, 1.5, 1.5, 1.5, 1));

    //Transform *tfLego = new Transform(0,0,0 , 3,0,-10, 0.5,0.5,0.5, 1);
    //createLego2x2(scene,tfLego, M_BLUE, true);

    Transform *tfLego1 = new Transform(0,0,0 , -1,0,-10, 1,1,1, 1);
    createLego4x1(scene,tfLego1, M_WHITE, true);

    //Transform *tCylinder = new Transform(0,0,0 , 0,0,-10, 1,1,1, 1);
    //createCylinder(scene, M_RED, tCylinder);

    LightBulb(scene, Vec(0,7,-10), Vec(1,1,1), 0.5, NULL);
    RayTracer renderer(camera, scene);// Create a RayTracer to render the scene from the camera location
    main_loop(renderer, NULL, true, true, true);// render the scene and react to keyboard events
}

