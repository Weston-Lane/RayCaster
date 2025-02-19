#include <iostream>
#include <vector>
#include <array>

#include "shaderUtils.h"
#include "GLinit.h"
#include "mathUtils.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "logger.h"

//TODO:ROTATION MOVEMENT AND GET A RAY CASTED.
float px=100,py=300,pa=0,pdx=cos(pa)*5,pdy=sin(pa)*5;
int mapX=8, mapY=8;
constexpr int mapS=64;

void setMap(VertexBuffer&, IndexBuffer&);

void updatePlayer(vec3f& player,vec4f& ray)
{
    //converts form pixel space to clip space
    player.x=px; player.y=py;
    ray.x=player.x; ray.y=player.y; ray.z=px+pdx*5; ray.w=py+pdy*5;
    player.clipSpace();
    //ray.x=(2*ray.x/WIDTH)-1; ray.y=1-(2*ray.y/HEIGHT); ray.z=(2*ray.z/WIDTH)-1; ray.w=1-(2*ray.w/HEIGHT);
    //ray.x=(2*ray.x/WIDTH)-1; ray.y=1-(2*ray.y/HEIGHT); ray.z=player.x+.1; ray.w=player.y;
    //ray.x=-.1; ray.y=-.1; ray.z=.1; ray.w=.1;
    //LOG_DEBUG(ray.x," ",ray.x," ",ray.z," ",ray.w);
    
    
    
    
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action==GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_D:
            {

            }break;
            case GLFW_KEY_A:
            {

            }break;
            case GLFW_KEY_W:
            {

            }break;
            case GLFW_KEY_S:
            {

            }break;
            case GLFW_KEY_ESCAPE:
            {

            }break;
        }    
    }

}
//TODO: PLAYER MOVEMENT IS RENDERED IN IMMEDIATE MODE (REBUFFERS PLAYER DATA EVERY FRAME). MAYBE CHANGE TO TRANSLATION MATRIX LATER
void processInput(GLFWwindow* window, vec3f& player, vec4f& ray)
{
    float speed=5.f;
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){pa-=0.1; if(pa<0) {pa+=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){pa+=0.1; if(pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){px+=pdx; py+=pdy;}
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){px-=pdx; py-=pdy;}
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){glfwSetWindowShouldClose(window,true);}
    updatePlayer(player,ray);
}
int main() 
{

    GLFWwindow* window;
    glInit(&window);
    //*geometry setup///////////////////////////////////////////////////////////////
    vec3f player(px,py,0);
    vec4f ray;
    player.clipSpace();
    
    unsigned int VAO[3];
    glGenVertexArrays(3,VAO);

    glBindVertexArray(VAO[0]);
    VertexBuffer playerVBO(&player,sizeof(player),GL_DYNAMIC_DRAW);//you only need to send the address of the vector since the member variables are laid out contigiously and are the first seen
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[2]);
    // VertexBuffer raysVBO(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
    // glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    // glEnableVertexAttribArray(3);

    float lineVertices[] = {
        -0.1f, -0.1f, // First vertex
         0.1f,  0.1f  // Second vertex
    };
    VertexBuffer test(&lineVertices,sizeof(lineVertices),GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(VAO[1]);
    VertexBuffer mapVBO;
    IndexBuffer mapEBO;
    setMap(mapVBO,mapEBO);

    Shader playerShader("../include/res/player.glsl");
    Shader mapShader("../include/res/map.glsl");
    Shader rayShader("../include/res/rays.glsl");

    //*Render Loop/////////////////////////////////////////////////////////////////////////////////////
    //vec3f pos(py,px,0);//*old render code to get players pos. keep just in case
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        mapShader.Bind();
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES,6*mapS,GL_UNSIGNED_INT,0);
        
        processInput(window,player,ray);

        playerVBO.Bind();
        playerVBO.BufferData(&player,sizeof(player),GL_DYNAMIC_DRAW);//this updates the vertex buffer every frame to get the players new pos//could use a translation matrix instead but this simplifies the process and is a small buffer anyways
        glBindVertexArray(VAO[0]);

        playerShader.Bind(); 
        glPointSize(10.f);
        glDrawArrays(GL_POINTS,0,1);

        //raysVBO.Bind();
        // LOG_DEBUG(ray.x,":",ray.y);
        // LOG_DEBUG(ray.z,":",ray.w);
        ray.x=-.1; ray.y=-.1; ray.z=.1; ray.w=.1;
        
        //raysVBO.BufferData(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
        
        test.Bind();
        glBindVertexArray(VAO[2]);
        rayShader.Bind();
        glLineWidth(3);
        glDrawArrays(GL_LINES,0,2);

        // float oldPos[2]={player.x,player.y};//*old code to get players pos. keep just in case
        // pos.x+=player.x-oldPos[0];
        // pos.y+=player.y-oldPos[1];
        //playerShader.SetUniform3f("translate",pos);

        glfwSwapInterval(3);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }
    //*Render Loop//////////////////////////////////////////////////////////////////////////////////////
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void setMap(VertexBuffer& mapVBO,IndexBuffer& mapEBO)
{
    std::array<ui8,mapS> mapN ={
        1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,1,
        1,0,1,0,0,1,0,1,
        1,0,0,1,0,1,0,1,
        1,0,0,0,0,1,0,1,
        1,0,0,1,1,1,0,1,
        1,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1
    };
    
    std::vector<vec3f>mapAttrib;
    mapAttrib.reserve(8*mapS);
    float xo,yo =0;
    float border=.005;
    float scaleW=2.6, scaleH=2.82;
    float mapSW=((float)mapS/WIDTH*scaleW);
    float mapSH=-((float)mapS/HEIGHT*scaleH);
    
    i8 ind=0;
    
    for(size_t y=0;y<mapY;y++)
    {
        for(size_t x=0;x<mapX;x++)
        {

            xo=(float)x*mapS; yo=(float)y*mapS;
            xo=(xo/(float)WIDTH*scaleW)-1; yo= 1-(yo/(float)HEIGHT*scaleH);
            
            if(mapN[y*mapX+x]==1)
            {
           
              mapAttrib.emplace_back(xo+border,yo,0.f);//pos
              mapAttrib.emplace_back(xo+mapSW,yo,0.f);
              mapAttrib.emplace_back(xo+border,yo+mapSH+border,0.f);
              mapAttrib.emplace_back(xo+mapSW,yo+mapSH+border,0.f);

              mapAttrib.emplace_back(1,1,1.f);//color
              mapAttrib.emplace_back(1,1,1.f);
              mapAttrib.emplace_back(1,1,1.f);
              mapAttrib.emplace_back(1,1,1.f);
            }
            else if(mapN[y*mapX+x]==0)
            {
              mapAttrib.emplace_back(xo+border,yo,0.f);//pos
              mapAttrib.emplace_back(xo+mapSW,yo,0.f);
              mapAttrib.emplace_back(xo+border,yo+mapSH+border,0.f);
              mapAttrib.emplace_back(xo+mapSW,yo+mapSH+border,0.f);

              mapAttrib.emplace_back(0,0,0.f);//color
              mapAttrib.emplace_back(0,0,0.f);//color
              mapAttrib.emplace_back(0,0,0.f);//color
              mapAttrib.emplace_back(0,0,0.f);//color
            }
            ind+=8; 
        }
    }
    std::vector<GLuint> indices;
    indices.reserve(6*mapS);
    for(GLuint i=0;i<8*mapS;i+=8)
    {
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+2);
        indices.push_back(i+1);
        indices.push_back(i+3);
        indices.push_back(i+2);
    }
    
    mapVBO.BufferData(mapAttrib.data(),mapAttrib.size()*sizeof(vec3f),GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)(4*sizeof(vec3f)));
    glEnableVertexAttribArray(2);
  
    mapEBO.BufferData(indices.data(),indices.size(),GL_STATIC_DRAW); 
}