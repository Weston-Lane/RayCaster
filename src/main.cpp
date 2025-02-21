#include <iostream>
#include <vector>
#include <array>

#include "shaderUtils.h"
#include "GLinit.h"
#include "mathUtils.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "logger.h"


float px=50,py=100,pa=0,pdx=0,pdy=0;
int mapX=8, mapY=8;
constexpr int mapS=64;
std::array<ui8,mapS> mapN ={
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void setMap(VertexBuffer&, IndexBuffer&);
//TODO: finish casting rays dont forget to put all values into clipSpace
void drawRays(vec3f& player, vec4f& ray)
{
    ui32 r,mx,my,mp,dof; float rx,ry,ra,xo,yo;
    ra=pa;
    for(r=0;r<1;r++)
    {
        //checking if the ray hits a horizontal line
        dof=0;
        float aTan=-1/tan(ra);
            //rays y pos rounded to 64th val //rayx by dist between playery and rayY *arcTan + playerx offset // calculates the next ray offset by sub next 64 pix and x offset
        if(ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px; yo=-64; xo=-yo*aTan;}//looking down, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        if(ra<PI){ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px; yo= 64; xo=-yo*aTan;}//looking up
        if(ra==0||ra==PI) {rx=px; ry=py; dof=8;}
        while(dof<8)
        {
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if(mp<mapX*mapY&&mapN[mp]==1){break;}
            else {rx+=xo; ry+=yo; dof++;}
        }
        ray.x=player.x; ray.y=player.y; ray.z=rx; ray.w=ry;
        //clipSpace(ray.x,ray.y); 
        clipSpace(ray.z,ray.w);
        //LOG_DEBUG(ray);
        //LOG_DEBUG(player);
    }
}
void updatePlayer(vec3f& player,vec4f& ray)
{
    //converts form pixel space to clip space
    player.x=px; player.y=py;
    player.clipSpace();
    // ray.x=player.x; ray.y=player.y; ray.z=px+pdx*2*5;ray.w=py+pdy*2*5;
    // clipSpace(ray.z,ray.w);
    
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
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){pa-=0.1; if(pa<0) {pa+=2*PI;} pdx=cos(pa)*2; pdy=sin(pa)*2;}
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){pa+=0.1; if(pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*2; pdy=sin(pa)*2;}
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

    glBindVertexArray(VAO[1]);
    VertexBuffer raysVBO(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(3);
    
    glBindVertexArray(VAO[2]);
    VertexBuffer mapVBO;
    IndexBuffer mapEBO;
    setMap(mapVBO,mapEBO);

    Shader playerShader("../include/res/player.glsl");
    Shader mapShader("../include/res/map.glsl");
    Shader rayShader("../include/res/rays.glsl");

    //*Render Loop/////////////////////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        mapShader.Bind();
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES,6*mapS,GL_UNSIGNED_INT,0);
        
        processInput(window,player,ray);
        drawRays(player,ray);
        playerVBO.Bind();
        playerVBO.BufferData(&player,sizeof(player),GL_DYNAMIC_DRAW);//this updates the vertex buffer every frame to get the players new pos//could use a translation matrix instead but this simplifies the process and is a small buffer anyways
        glBindVertexArray(VAO[0]);

        playerShader.Bind(); 
        glPointSize(10.f);
        glDrawArrays(GL_POINTS,0,1);

        raysVBO.Bind();
        raysVBO.BufferData(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
        glBindVertexArray(VAO[1]);

        rayShader.Bind();
        glLineWidth(3);//!glLineWidth is deprecated on certain hardware. Intel Iris is one of them
        glLineWidth(1);
        glDrawArrays(GL_LINES,0,2);
        
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

    
    std::vector<vec3f>mapAttrib;
    mapAttrib.reserve(8*mapS);
    float xo,yo =0;
    float border=.002;
    float scale=1; //scaleH=1;//2.6 //2.82
    //TODO: figure out the scaling. only works on 1024x512 ratio.
    float mapSW=((float)mapS/WIDTH*2);//64 as an abs value in clip space but not a pos
    float mapSH=-((float)mapS/HEIGHT*2);
    //LOG_DEBUG(mapSW," ", mapSH);
    
    i8 ind=0;
    
    for(size_t y=0;y<mapY;y++)
    {
        for(size_t x=0;x<mapX;x++)
        {

            xo=(float)x*mapS; yo=(float)y*mapS;
            clipSpace(xo,yo);//xo=(xo/(float)WIDTH*scaleW)-1; yo= 1-(yo/(float)HEIGHT*scaleH);
            
            if(mapN[y*mapX+x]==1)
            {
           
              mapAttrib.emplace_back(xo,yo,0.f);//pos//top left
              mapAttrib.emplace_back(xo+mapSW-border,yo,0.f);//top right
              mapAttrib.emplace_back(xo,yo+mapSH+border,0.f);//bottom left
              mapAttrib.emplace_back(xo+mapSW-border,yo+mapSH+border,0.f);//bottom right

              mapAttrib.emplace_back(1,1,1.f);//color
              mapAttrib.emplace_back(1,1,1.f);
              mapAttrib.emplace_back(1,1,1.f);
              mapAttrib.emplace_back(1,1,1.f);
            }
            else if(mapN[y*mapX+x]==0)
            {
              mapAttrib.emplace_back(xo,yo,0.f);//pos
              mapAttrib.emplace_back(xo+mapSW-border,yo,0.f);
              mapAttrib.emplace_back(xo,yo+mapSH+border,0.f);
              mapAttrib.emplace_back(xo+mapSW-border,yo+mapSH+border,0.f);

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