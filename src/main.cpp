#include <vector>
#include "GLinit.hpp"
#define GEO GL_POINTS


void setMap(const unsigned int, unsigned int);

void updatePlayer(float (&player)[3], float px, float py)
{
    //converts form pixel space to clip space
    player[0]+= ((px/WIDTH));
    player[1]+= -((py/HEIGHT));
    player[2]=0;
}
void processInput(GLFWwindow* window,float (&player)[3])
{
    float speed=5.f;
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){updatePlayer(player,speed,0);}
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){updatePlayer(player,-speed,0);}
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){updatePlayer(player,0,-speed);}
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){updatePlayer(player,0,speed);}
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){glfwSetWindowShouldClose(window,true);}
}
int main() 
{
    
    GLFWwindow* window;
    glInit(&window);
    //*geometry setup///////////////////////////////////////////////////////////////
    float player[3]={300,300,0};
    clipSpace(player[0],player[1],player[2]);
    updatePlayer(player,player[0],player[1]);
    
    unsigned int VBO, VAO[2], EBO;
    unsigned int mapBuffer;

    glGenVertexArrays(2,VAO);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(player),player,GL_DYNAMIC_DRAW);
    //*setting up attrib///////////////////////////////////////////////////////
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&mapBuffer);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER,mapBuffer);
    glGenBuffers(1,&EBO);
    
    setMap(mapBuffer,EBO);

    //*shader setup//////////////////////////////////////////////////////////////
    //*vert shader//////////////////////////////////////////////////////
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSourcePlayer,NULL);
    glCompileShader(vertexShader);

    int success=0;
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        LOG_ERROR("VERTEX SHADER COMPILATION FAILED [GL infoLog]:",infoLog);
    }

    unsigned int mapVShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mapVShader,1,&vertexSourceMap,NULL);
    glCompileShader(mapVShader);

   
    glGetShaderiv(mapVShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(mapVShader,512,NULL,infoLog);
        LOG_ERROR("VERTEX SHADER COMPILATION FAILED [GL infoLog]:",infoLog);
    }
    //*frag shader//////////////////////////////////////////////////////
    unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSourcePlayer,NULL);
    glCompileShader(fragmentShader);
 
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        LOG_ERROR("FRAGMENT SHADER COMPILATION FAILED [GL infoLog]:",infoLog);
    }

    unsigned int mapFShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mapFShader,1,&fragmentSourceMap,NULL);
    glCompileShader(mapFShader);
 
    glGetShaderiv(mapFShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(mapFShader,512,NULL,infoLog);
        LOG_ERROR("FRAGMENT SHADER COMPILATION FAILED [GL infoLog]:",infoLog);
    }
    //*Creating shader program//////////////////////////////////////////////////////
    unsigned int shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glGetShaderiv(shaderProgram,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderProgram,512,NULL,infoLog);
        LOG_ERROR("SHADER COULD NOT LINK [GL infoLog]:",infoLog);
    }

    unsigned int shaderProgramMap=glCreateProgram();
    glAttachShader(shaderProgramMap,mapVShader);
    glAttachShader(shaderProgramMap,mapFShader);
    glLinkProgram(shaderProgramMap);

    glGetShaderiv(shaderProgramMap,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderProgramMap,512,NULL,infoLog);
        LOG_ERROR("SHADER COULD NOT LINK [GL infoLog]:",infoLog);
    }
    //*Creating shader program//////////////////////////////////////////////////////
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //*shader setup//////// //////////////////////////////////////////////////////

    // glfwGetFramebufferSize(window, &width, &height);
    // framebuffer_size_callback(window, width, height);
    
    float pos[2]={0,0};
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgramMap);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES,6*64,GL_UNSIGNED_INT,0);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glPointSize(10.f);
        glDrawArrays(GEO,0,1);
        float oldPos[2]={player[0],player[1]};
        processInput(window,player);
        int vertexPos=glGetUniformLocation(shaderProgram,"translate");
        //std::cout<<player[0]<<" "<<oldPos[0]<<std::endl;
        pos[0]+=player[0]-oldPos[0];
        pos[1]+=player[1]-oldPos[1];
        glUniform3f(vertexPos,pos[0],pos[1],0);

   
        //glDrawArrays(GL_TRIANGLES,0,8*64);
        glfwSwapInterval(2);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void setMap(const unsigned int mapBuffer, unsigned int EBO)
{
    
    int mapX=8, mapY=8, mapS=mapY*mapX;
    int mapN[64]={
        1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,1,0,0,1,
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
            //TODO:figure out how to draw lines in between tiles
              mapAttrib.emplace_back(vec3f(xo+border,yo,0.f));//pos
              mapAttrib.emplace_back(vec3f(xo+mapSW,yo,0.f));
              mapAttrib.emplace_back(vec3f(xo+border,yo+mapSH+border,0.f));
              mapAttrib.emplace_back(vec3f(xo+mapSW,yo+mapSH+border,0.f));

              mapAttrib.emplace_back(vec3f(1,1,1.f));//color
              mapAttrib.emplace_back(vec3f(1,1,1.f));
              mapAttrib.emplace_back(vec3f(1,1,1.f));
              mapAttrib.emplace_back(vec3f(1,1,1.f));
            }
            else if(mapN[y*mapX+x]==0)
            {
              mapAttrib.emplace_back(vec3f(xo+border,yo,0.f));//pos
              mapAttrib.emplace_back(vec3f(xo+mapSW,yo,0.f));
              mapAttrib.emplace_back(vec3f(xo+border,yo+mapSH+border,0.f));
              mapAttrib.emplace_back(vec3f(xo+mapSW,yo+mapSH+border,0.f));

              mapAttrib.emplace_back(vec3f(0,0,0.f));//color
              mapAttrib.emplace_back(vec3f(0,0,0.f));//color
              mapAttrib.emplace_back(vec3f(0,0,0.f));//color
              mapAttrib.emplace_back(vec3f(0,0,0.f));//color
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
    
    #define DEBU
    #ifdef DEBUG==1
    int index=0;
    for(int y=0;y<8*mapS;y++)
    {
        if(y%8==0){std::cout<<std::endl;};
        std::cout<<mapAttrib[y]<<" ";
        
    }
    #endif
    
    
    glBufferData(GL_ARRAY_BUFFER,mapAttrib.size()*sizeof(vec3f),mapAttrib.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint),indices.data(),GL_STATIC_DRAW);
    //*setting up attrib///////////////////////////////////////////////////////
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(vec3f),(void*)(4*sizeof(vec3f)));
    glEnableVertexAttribArray(2);
  
    
}