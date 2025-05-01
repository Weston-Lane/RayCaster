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
#include "vertexArray.h"

float px=340,py=200,pa=20.5,pdx=0,pdy=0;
int mapX=16, mapY=16;
constexpr int mapS=256;
const double FPS=60.0;
const double FRAME_TIME=1.0/FPS;

struct Sprite
{
    vec3f pos;
};

std::array<i8,mapS> mapWalls ={
    // greyStone,//0
    // wood,//1
    // redBrick,//2
    // eagle//3
    // mossy//4
    

    0, 3, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2,
    3, -1, 3, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 1, 0, 1,
    3, -1, 3, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
    2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    2, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    2, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    2, 2, 2, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 2, 2, 2, 
    1, 1, 1, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 2, 2, 2, 
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
    2, 1, 1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

std::array<i8,mapS> mapFloor ={
    // greyStone,//0
    // wood,//1
    // redBrick,//2
    // eagle//3
    // mossy//4
    

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};
std::array<i8,mapS> mapCeiling ={
    // greyStone,//0
    // wood,//1
    // redBrick,//2
    // eagle//3
    // mossy//4
    

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

void setMap(VertexBuffer&, IndexBuffer&);

float dist(float ax, float ay, float bx, float by)
{
    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}
//TODO:implement 2D TextureArray. This allows the TexArray to be passed in as a Sampler 2D uniform which hold multiple textures and the layer (which texture) can be selected via a vertex attribute
void drawTerrain(std::vector<vec2f>& walls, VertexBuffer& wallVBO, ui32 wallVAO, 
                 std::vector<vec2f>& floors, VertexBuffer& floorVBO, ui32 floorVAO, ui8 lineWidth)
{
        //textures[0].Bind(0);
        wallVBO.Bind();
        wallVBO.BufferData(walls.data(),walls.size()*sizeof(vec2f),GL_DYNAMIC_DRAW);
        glBindVertexArray(wallVAO);
        glLineWidth(lineWidth); glDrawArrays(GL_LINES,0,walls.size()/3);//draws rays

        //textures[1].Bind(0);
        floorVBO.Bind();
        glBindVertexArray(floorVAO);
        floorVBO.BufferData(floors.data(),floors.size()*sizeof(vec2f),GL_DYNAMIC_DRAW);
        glPointSize(lineWidth); glDrawArrays(GL_POINTS,0,floors.size()/3);//draws rays
}
void drawRays(VertexBuffer& wallVBO, ui32 wallVAO, 
                VertexBuffer& floorVBO, ui32 floorVAO)
{
    ui8 blockSide=8;//exponet for a power of 2, so each block is 64x64 pixels, 2^6
    i16 blockSidePx=pow(2,blockSide);
    ui16 numRays=200;
    ui8 depthOfRay=20;
    ui8 lineWidth=10;
    float rayPerDeg=ONE_DGR/4;
    float rayOff=1/rayPerDeg*ONE_DGR*25;//(one deg/3*x=one deg*30)//formula for having a 30 deg offset no matter the rayPerDeg

    vec4f texCoord;
    vec4f ray; 
    std::vector<vec2f> walls; 
    std::vector<vec2f> floors;
    walls.reserve(2*WIDTH);
    floors.reserve(2*WIDTH);
    ui8 texSlot=0;

    //mp,mv,mh are used to determine the number in the map array to let the shader know what texture to sample from
    i32 r,mx,my,mp,mv,mh,dof,tileInd=0; 
    double rx,ry,ra,xo,yo,distWall=0;
    ra=pa-rayPerDeg*rayOff;
    
 

    if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;}

    for(r=0;r<numRays;r++)
    {
        bool vert_hor=true;
        float distH=FLT_MAX, distV=FLT_MAX, hx=px, hy=py, vx=px, vy=py;
        //checking if the ray hits a horizontal line
        dof=0;
        float aTan=-1/tan(ra);
        //rays y pos rounded to 64th val //rayx by dist between playery and rayY *arcTan + playerx offset // calculates the next ray offset by sub next 64 pix and x offset
        if(ra>PI)     {ry=(((int)py>>blockSide)<<blockSide)-0.0001; rx=(py-ry)*aTan+px; yo=-blockSidePx; xo=-yo*aTan;}//looking down, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        else if(ra<PI){ry=(((int)py>>blockSide)<<blockSide)+blockSidePx;     rx=(py-ry)*aTan+px; yo=blockSidePx;  xo=-yo*aTan;}//looking up
        else          {rx=px; ry=py; dof=depthOfRay;}
        while(dof<depthOfRay)
        {
            mx=((int)rx>>blockSide); my=(int)(ry)>>blockSide; mp=my*mapX+mx;
            
            if(mp>=0 && mp<mapX*mapY && mapWalls[mp]>=1){distH=dist(px,py,rx,ry); mh=mapWalls[mp]; break;}
            else {rx+=xo; ry+=yo; dof++;}
        }
        hx=rx; hy=ry; 
        
        //checking if the ray hits a vertical line
        dof=0;
        float nTan=-tan(ra);
        //rays y pos rounded to 64th val //rayx by dist between playery and rayY *arcTan + playerx offset // calculates the next ray offset by sub next 64 pix and x offset
        if(ra>PI2&&ra<PI3)     {rx=(((int)px>>blockSide)<<blockSide)-0.0001;                    ry=(px-rx)*nTan+py; xo=-blockSidePx; yo=-xo*nTan;}//looking left, >>6 divides by 64, <<6 mult by 64 to round to nearest 64th val. -0.00001 for accuracy
        else if(ra<PI2||ra>PI3){rx=(((int)px>>blockSide)<<blockSide)+blockSidePx;               ry=(px-rx)*nTan+py; xo=blockSidePx;  yo=-xo*nTan;}//looking right
        else                   {rx=px; ry=py; dof=depthOfRay;}
        while(dof<depthOfRay)
        {
            mx=(int)(rx)>>blockSide; my=(int)(ry)>>blockSide; mp=my*mapX+mx;
            
            if(mp>=0 && mp<mapX*mapY && mapWalls[mp]>=1){distV=dist(px,py,rx,ry); mv=mapWalls[mp]; break;}
            else {rx+=xo; ry+=yo; dof++;}
        }
        vx=rx; vy=ry;

        if(distV<distH){rx=vx; ry=vy; distWall=distV; vert_hor=0; tileInd=mv;}
        else {rx=hx;ry=hy; distWall=distH;vert_hor=1; tileInd=mh;}

        //distance increases fog
        float fogScale=1-distWall/(float)(WIDTH);

        //(vert_hor) ? glUniform4f(wallColorLoc,0.7,0.7,0.8,1.f*fogScale) : glUniform4f(wallColorLoc,0.6,0.6,0.7,1.f*fogScale);
        float texX=0;
        (vert_hor) ? texX=(float)((int)rx%blockSidePx)/blockSidePx : texX=(float)((int)ry%blockSidePx)/blockSidePx;
        texCoord.x=texX; texCoord.y=0; texCoord.z=texX; texCoord.w=1;
    
        //fixing fisheye
        float normDist=pa-ra; if(normDist<0){normDist+=2*PI;} if(normDist>2*PI){normDist-=2*PI;}
        distWall*=cos(normDist);

        //drawing walls
        float wallH=(mapS*HEIGHT)/distWall;
        float offset=HEIGHT/2-wallH/2;
        if(wallH>HEIGHT*mapS){wallH=HEIGHT*mapS;}

               //wall top (x,y)                         //wall bottom (x,y)
        ray.x=r*lineWidth; ray.y=offset; ray.z=r*lineWidth; ray.w=wallH+offset;
        clipSpace(ray.x,ray.y); clipSpace(ray.z,ray.w);
        walls.emplace_back(ray.x,ray.y); walls.emplace_back(texCoord.x,texCoord.y); walls.emplace_back(tileInd,fogScale); //walls.emplace_back(distWall,WIDTH);
        walls.emplace_back(ray.z,ray.w); walls.emplace_back(texCoord.z,texCoord.w); walls.emplace_back(tileInd,fogScale); //walls.emplace_back(distWall,WIDTH);
        
        //checks for dist to see if cielings and floors should be generated
        if(wallH>20)
        {
            //draw floors
            for(int y=offset+wallH;y<HEIGHT;y++)
            {
                float ta=pa-ra;
                float dy=y-(HEIGHT/2), deg=-ra, fishFix=cos(ta);
                float tx=px/4.4 + cos(deg)*474*64/(dy*fishFix);
                float ty=py/4.4 - sin(deg)*474*64/(dy*fishFix);
                ray.x=r*lineWidth; ray.y=y;
                mx=(int)tx/(blockSidePx/4-6); my=(int)ty/(blockSidePx/4-6); tileInd=mapFloor[my*mapX+mx];//TODO: im  not sure why but dividing by the bock face size of 64 is too much and need to be offset by 6pixels
                clipSpace(ray.x,ray.y);

                texCoord.x=(float)((int)tx%64)/64; texCoord.y=(float)((int)ty%64)/64;//texture size is 64x64
                fogScale=dy/(HEIGHT*7);
                floors.emplace_back(ray.x,ray.y); floors.emplace_back(texCoord.x,texCoord.y); floors.emplace_back(tileInd,fogScale); //floors.emplace_back(distWall,WIDTH);
                        
            }
            //draw cielings
            for(int y=0;y<offset;y++)
            {
                float ta=pa-ra;
                float dy=(HEIGHT/2)-y, deg=-ra, raFix=cos(ta);
                float tx=px/4.4 + cos(deg)*474*64/dy/raFix;
                float ty=py/4.4 - sin(deg)*474*64/dy/raFix;

                ray.x=r*lineWidth; ray.y=y;
                mx=(int)tx/(blockSidePx/4-6); my=(int)ty/(blockSidePx/4-6); tileInd=mapCeiling[my*mapX+mx];
                clipSpace(ray.x,ray.y);
                
                texCoord.x=(float)((int)tx%64)/64; texCoord.y=(float)((int)ty%64)/64; 
                fogScale=dy/(HEIGHT*7);
                floors.emplace_back(ray.x,ray.y); floors.emplace_back(texCoord.x,texCoord.y); floors.emplace_back(tileInd,fogScale); //floors.emplace_back(distWall,WIDTH);
            }
        }

        

        ra+=rayPerDeg; if(ra<0){ra+=2*PI;} if(ra>2*PI){ra-=2*PI;}

    }

    drawTerrain(walls,wallVBO,wallVAO,floors,floorVBO,floorVAO,lineWidth);
}
// void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
// }
//TODO: PLAYER MOVEMENT IS RENDERED IN IMMEDIATE MODE (REBUFFERS PLAYER DATA EVERY FRAME). MAYBE CHANGE TO TRANSLATION MATRIX LATER
void processInput(GLFWwindow* window)
{
    float speed=5.f;
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){pa-=0.035; if(pa<0) {pa+=2*PI;} pdx=cos(pa)*2; pdy=sin(pa)*2;}
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){pa+=0.035; if(pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*2; pdy=sin(pa)*2;}
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){px+=pdx*speed; py+=pdy*speed;}
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){px-=pdx*speed; py-=pdy*speed;}
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){glfwSetWindowShouldClose(window,true);}
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
    //player.clipSpace();
    vec4f ray;
    
    unsigned int VAO[4];
    glGenVertexArrays(4,VAO);

    glBindVertexArray(VAO[0]);
    VertexBuffer playerVBO(&player,sizeof(player),GL_DYNAMIC_DRAW);//you only need to send the address of the vector since the member variables are laid out contigiously and are the first seen
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    VertexBuffer wallsVBO; //dont know the size yet. .BufferData() later in draw ray function
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)0);//drawn with gl lines layout: (p1,p2|tx1,tx2|texSlotx,textSloty/FogScale) //texSloty is not used, it is just there so it can be a vec2f for the buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)sizeof(vec2f));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)(2*sizeof(vec2f)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAO[2]);//dont know the size yet. .BufferData() later in draw ray function
    VertexBuffer floorVBO;//floors are drawn with gl point layout (p1,p2|tx1,tx2) then cieling (p1,p2|tx1,tx2|texSlotx texSloty/FogScale) 
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)0);//drawn with gl lines layout: (p1,p2|tx1,tx2|texSlotx,textSloty/FogScale) //texSloty is not used, it is just there so it can be a vec2f for the buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)sizeof(vec2f));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vec2f)*3,(void*)(2*sizeof(vec2f)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(VAO[3]);
    VertexBuffer mapVBO;
    IndexBuffer mapEBO;
    setMap(mapVBO,mapEBO);

    Shader playerShader("../include/res/player.glsl");
    Shader mapShader("../include/res/map.glsl");
    Shader terrainShader("../include/res/terrain.glsl");

    const int numTex=5;
    Texture2D wood("../assets/wood.png");//cannot create textures before gl context
    Texture2D greyStone("../assets/greystone.png");//cannot create textures before gl context
    Texture2D redBrick("../assets/redBrick.png");
    Texture2D eagle("../assets/eagle.png");
    Texture2D mossy("../assets/mossy.png");
    //!THE ORDER OF THE TEXURES IN THIS ARRAY IS THE CORRESPONDING MAP NUMBER IN THE MAP ARRAY.
    Texture2D textures[numTex]={
        greyStone,//0
        wood,//1
        redBrick,//2
        eagle,//3
        mossy //4
    };

    TextureArray2D texArray(textures, wood.GetWidth(), wood.GetHeight(), numTex);
    
    Sprite sprite; sprite.pos.x=340; sprite.pos.y=200;sprite.pos.z=0;



    
    //locks fps to 60

    double prvTime=glfwGetTime();

    //*Render Loop/////////////////////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        //*locks fps to set fps/////////////////
        double currTime=glfwGetTime();
        float deltaTime=currTime-prvTime;
        if(deltaTime<FRAME_TIME) {std::this_thread::sleep_for(std::chrono::duration<double>(FRAME_TIME-(deltaTime)));}
        prvTime+=FRAME_TIME;
        //*locks fps to set fps/////////////////

        
        processInput(window);
        

        terrainShader.Bind();
        texArray.Bind(1,terrainShader.getID(),"uTexArray");
        //terrainShader.SetUniform1i("uTexture",0);

        drawRays(wallsVBO,VAO[1],floorVBO,VAO[2]);

        mapShader.Bind();
        glBindVertexArray(VAO[3]);
        glDrawElements(GL_TRIANGLES,6*mapS,GL_UNSIGNED_INT,0);

        vec3f spriteRender;
        spriteRender.x=sprite.pos.x-px; spriteRender.y=sprite.pos.y-py; spriteRender.z=sprite.pos.z;
        float cs=cos(pa), sn=sin(pa);
        float a=spriteRender.y*cs+spriteRender.x*sn;
        float b=spriteRender.x*cs-spriteRender.y*sn;
        spriteRender.x=a; spriteRender.y=b;
        spriteRender.x=(spriteRender.x*108/spriteRender.y)+(120/2);
        spriteRender.y=(spriteRender.z*108/spriteRender.y)+(80/2);
        spriteRender.clipSpace();
        
        playerVBO.Bind();
        playerVBO.BufferData(&spriteRender,sizeof(spriteRender),GL_DYNAMIC_DRAW);//this updates the vertex buffer every frame to get the players new pos//could use a translation matrix instead but this simplifies the process and is a small buffer anyways
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
    float scale=.06;//(float)(mapX)/mapS;
    //TODO: figure out the scaling. only works on 1024x512 ratio.
    float mapSW=((float)mapS/WIDTH*2)*scale;//64 as an abs value in clip space but not a pos
    float mapSH=-((float)mapS/HEIGHT*2)*scale;// *2 since clip space goes from [-1,1]
    
    i8 ind=0;
    for(size_t y=0;y<mapY;y++)
    {
        for(size_t x=0;x<mapX;x++)
        {

            xo=(float)x*mapS*scale; yo=(float)y*mapS*scale;
            clipSpace(xo,yo);
            
            if(mapWalls[y*mapX+x]>=1)
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
            else if(mapWalls[y*mapX+x]==0)
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
    
    mapVBO.BufferData(mapAttrib.data(),mapAttrib.size()*sizeof(vec3f),GL_STATIC_DRAW); //layout (p1,p2,p3,p4|c1,c2,c3,c4)
    mapEBO.BufferData(indices.data(),indices.size(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)(4*sizeof(vec3f)));
    glEnableVertexAttribArray(1);
  
     
}
