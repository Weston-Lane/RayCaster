#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cfloat>

#include "shaderUtils.h"
#include "GLinit.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "logger.h"
#include "texture.h"

float px=200,py=100,pa=0,pdx=0,pdy=0;
int mapX=8, mapY=8;
constexpr int mapS=64;
const double FPS=80.0;
const double FRAME_TIME=1.0/FPS;

std::array<ui8,mapS> mapN ={
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,1,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
    // 0,1,1,1,1,1,1,0,
    // 0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,
    // 0,1,1,1,1,1,1,0
};
float dist(float ax, float ay, float bx, float by)
{
    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}
void setMap(VertexBuffer&, IndexBuffer&);

//TODO: get seperate wall colors
void drawRays(vec3f& player, vec4f& ray, VertexBuffer& rayVBO, Shader& rayShader, unsigned int VAO)
{
    int blockSide=6;//exponet for a power of 2, so each block is 64x64 pixels, 2^6
    int wallColorLoc=glGetUniformLocation(rayShader.getID(),"wallColor");
    int texCoordLoc=glGetUniformLocation(rayShader.getID(),"texCoord");
    int wallIncLoc=glGetUniformLocation(rayShader.getID(),"y");
    int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,distWall;
    ra=pa-ONE_DGR*30; if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;}

    for(r=0;r<60;r++)
    {
        bool vert_hor=1;
        float distH=FLT_MAX, distV=FLT_MAX, hx=px, hy=py, vx=px, vy=py;
        //checking if the ray hits a horizontal line
        dof=0;
        float aTan=-1/tan(ra);
        //rays y pos rounded to 64th val //rayx by dist between playery and rayY *arcTan + playerx offset // calculates the next ray offset by sub next 64 pix and x offset
        if(ra>PI)     {ry=(((int)py>>blockSide)<<blockSide)-0.0001; rx=(py-ry)*aTan+px; yo=-pow(2,blockSide); xo=-yo*aTan;}//looking down, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        else if(ra<PI){ry=(((int)py>>blockSide)<<blockSide)+pow(2,blockSide);     rx=(py-ry)*aTan+px; yo=pow(2,blockSide);  xo=-yo*aTan;}//looking up
        // if(ra>PI)     {ry=(((int)py>>5)<<5)-0.0001; rx=(py-ry)*aTan+px; yo=-32; xo=-yo*aTan;}//looking down, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        // else if(ra<PI){ry=(((int)py>>5)<<5)+32;     rx=(py-ry)*aTan+px; yo=32;  xo=-yo*aTan;}//looking up
        else          {rx=px; ry=py; dof=8;}
        while(dof<8)
        {
            
            mx=((int)rx>>blockSide); my=(int)(ry)>>blockSide; mp=my*mapX+mx;
            //mx=((int)rx>>5); my=(int)(ry)>>5; mp=my*mapX+mx;
            
            if(mp>=0 && mp<mapX*mapY && mapN[mp]==1){distH=dist(px,py,rx,ry); break;}
            else {rx+=xo; ry+=yo; dof++;}
        }
        hx=rx; hy=ry; 
        
        //checking if the ray hits a vertical line
        dof=0;
        float nTan=-tan(ra);
        //rays y pos rounded to 64th val //rayx by dist between playery and rayY *arcTan + playerx offset // calculates the next ray offset by sub next 64 pix and x offset
        if(ra>PI2&&ra<PI3)     {rx=(((int)px>>blockSide)<<blockSide)-0.0001;                    ry=(px-rx)*nTan+py; xo=-pow(2,blockSide); yo=-xo*nTan;}//looking left, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        else if(ra<PI2||ra>PI3){rx=(((int)px>>blockSide)<<blockSide)+pow(2,blockSide);          ry=(px-rx)*nTan+py; xo=pow(2,blockSide);  yo=-xo*nTan;}//looking right
        // if(ra>PI2&&ra<PI3)     {rx=(((int)px>>5)<<5)-0.0001;      ry=(px-rx)*nTan+py; xo=-32; yo=-xo*nTan;}//looking left, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        // else if(ra<PI2||ra>PI3){rx=(((int)px>>5)<<5)+32;          ry=(px-rx)*nTan+py; xo=32;  yo=-xo*nTan;}//looking right
        else                   {rx=px; ry=py; dof=8;}
        while(dof<8)
        {
            mx=(int)(rx)>>blockSide; my=(int)(ry)>>blockSide; mp=my*mapX+mx;
            //mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;
            if(mp>=0 && mp<mapX*mapY && mapN[mp]==1){distV=dist(px,py,rx,ry); break;}
            else {rx+=xo; ry+=yo; dof++;}
        }
        vx=rx; vy=ry; //check

        if(distV<distH){rx=vx; ry=vy; distWall=distV; vert_hor=0;}
        else {rx=hx;ry=hy; distWall=distH;vert_hor=1;}
        
        ray.x=player.x; ray.y=player.y; ray.z=rx; ray.w=ry;
        clipSpace(ray.z,ray.w);

        rayVBO.Bind();
        rayVBO.BufferData(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
        glBindVertexArray(VAO);
        glUniform4f(wallColorLoc,0.2,0.1,0.1,1);
        glLineWidth(1);//!glLineWidth is deprecated on certain hardware. Intel Iris is one of them
        glDrawArrays(GL_LINES,0,2);//draws rays
        //distance increases fog
        
        float fogScale=1-distWall/(float)(WIDTH/1.5);
        (vert_hor) ? glUniform4f(wallColorLoc,0.7,0.7,0.8,1.f*fogScale) : glUniform4f(wallColorLoc,0.6,0.6,0.7,1.f*fogScale);
        
        //(vert_hor) ? glUniform2f(texCoordLoc,.5,0) : glUniform2f(texCoordLoc,.5,0);
        
        //fixing fisheye
        float normDist=pa-ra; if(normDist<0){normDist+=2*PI;} if(normDist>2*PI){normDist-=2*PI;}
        distWall*=cos(normDist);

        //drawing walls
        float wallH=(mapS*320)/distWall;
        float offset=160-wallH/2;
        if(wallH>320*mapS){wallH=320*mapS;}

        ray.x=r*8+530; ray.y=offset; ray.z=r*8+530; ray.w=wallH+offset;
        //ray.x=r*10+400; ray.y=offset; ray.z=r*10+400; ray.w=wallH+offset;q
        // LOG_DEBUG(rx," : ",(int)rx%64);
        // LOG_DEBUG(ry," : ",(int)ry%64);
        clipSpace(ray.x,ray.y); clipSpace(ray.z,ray.w);
        //float wall[2]={ray.x,0};
        rayVBO.Bind();
        rayVBO.BufferData(&ray,sizeof(ray),GL_DYNAMIC_DRAW);
        //rayVBO.BufferData(&wall,sizeof(wall),GL_DYNAMIC_DRAW);
        glBindVertexArray(VAO);
        // for(float y=0;y<wallH;y++)
        // {   
        //     glUniform2f(wallIncLoc,ray.x, y+offset);
        //     glPointSize(8); glDrawArrays(GL_POINTS,0,1);//draws rays  //glDrawArrays(GL_POINTS,0,2);//draws rays  
        // }

        //glLineWidth(10);glDrawArrays(GL_LINES,0,2);//draws rays
        glLineWidth(8);glDrawArrays(GL_LINES,0,2);//draws rays
        //glPointSize(8); glDrawArrays(GL_POINTS,0,1);//draws rays  //glDrawArrays(GL_POINTS,0,2);//draws rays  
        ra+=ONE_DGR; if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;}
    }
}
void updatePlayer(vec3f& player,vec4f& ray)
{
    player.x=px; player.y=py;
    player.clipSpace();   
}
// void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if(action==GLFW_PRESS)
//     {
//         switch(key)
//         {
//             case GLFW_KEY_D:
//             {

//             }break;
//             case GLFW_KEY_A:
//             {

//             }break;
//             case GLFW_KEY_W:
//             {

//             }break;
//             case GLFW_KEY_S:
//             {

//             }break;
//             case GLFW_KEY_ESCAPE:
//             {

//             }break;
//         }    
//     }

// }
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
void setFrameRate(double fps)
{
    
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

    Texture2D redBrick("../assets/redbrick.png");//cannot create textures before gl context

    //locks fps to 60

    double prvTime=glfwGetTime();

    //*Render Loop/////////////////////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        //*locks fps to set fps/////////////////
        double currTime=glfwGetTime();
        if(currTime-prvTime<FRAME_TIME) {std::this_thread::sleep_for(std::chrono::duration<double>(FRAME_TIME-(currTime-prvTime)));}
        prvTime+=FRAME_TIME;
        //*locks fps to set fps/////////////////

        
        processInput(window,player,ray);
        

        rayShader.Bind();

        redBrick.Bind(0);
        rayShader.SetUniform1i("uTexture",0);

        drawRays(player,ray,raysVBO,rayShader,VAO[1]);

        mapShader.Bind();
        // redBrick.Bind(0);
        // mapShader.SetUniform1i("uTexture",0);

        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES,6*mapS,GL_UNSIGNED_INT,0);

        playerVBO.Bind();
        playerVBO.BufferData(&player,sizeof(player),GL_DYNAMIC_DRAW);//this updates the vertex buffer every frame to get the players new pos//could use a translation matrix instead but this simplifies the process and is a small buffer anyways
        glBindVertexArray(VAO[0]);

        playerShader.Bind(); 
        glPointSize(10.f);
        glDrawArrays(GL_POINTS,0,1);
        //prvTime=glfwGetTime();
        glfwSwapInterval(2);
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
    float mapSW=((float)mapS/WIDTH*2)*scale;//64 as an abs value in clip space but not a pos
    float mapSH=-((float)mapS/HEIGHT*2)*scale;// *2 since clip space goes from [-1,1]
    //LOG_DEBUG(mapSW," ", mapSH);
    
    i8 ind=0;
    
    for(size_t y=0;y<mapY;y++)
    {
        for(size_t x=0;x<mapX;x++)
        {

            xo=(float)x*mapS*scale; yo=(float)y*mapS*scale;
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
            // mapAttrib.emplace_back(0,1,1.f);//color
            // mapAttrib.emplace_back(1,1,1.f);
            // mapAttrib.emplace_back(0,0,1.f);
            // mapAttrib.emplace_back(1,0,1.f);
            }
            else if(mapN[y*mapX+x]==0)
            {
              mapAttrib.emplace_back(xo,yo,0.f);//pos
              mapAttrib.emplace_back(xo+mapSW,yo,0.f);
              mapAttrib.emplace_back(xo,yo+mapSH,0.f);
              mapAttrib.emplace_back(xo+mapSW,yo+mapSH,0.f);

              mapAttrib.emplace_back(.5,.5,.5);//color
              mapAttrib.emplace_back(.5,.5,.5);//color
              mapAttrib.emplace_back(.5,.5,.5);//color
              mapAttrib.emplace_back(.5,.5,.5);//color
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
