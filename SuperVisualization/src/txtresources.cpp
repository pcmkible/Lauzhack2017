#include"brate/txtresources.h"

GLuint fontTexture;

#include "brate/linmath.h"

void StartFontTexture(void){
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, "resources/myaheiui.png");
    //unsigned error = lodepng::decode(image, width, height, "res/mvboli.png");

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...


    std::cout << "Hello world" << std::endl;
    glGenTextures(1, &fontTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    //glBindTexture(GL_TEXTURE_2D, fontTexture);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    std::cout << "Hello world2" << std::endl;

    image.clear();

    /*
    error = lodepng::decode(image, width, height, "res/tbunchecked.png");

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...


    glActiveTexture(GL_TEXTURE0);
    glGenTextures( 1, &uncheckedTexture );
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, uncheckedTexture );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    // build our texture mipmaps depending on system architecture

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    */

    //image.clear();
}

void StringWithLimits::setPosition(float pPosition[2]){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    mat4x4 scaleMatrix;
    mat4x4_identity(scaleMatrix);
    mat4x4_scale_aniso(scaleMatrix, scaleMatrix, this->textScale, this->textScale,this->textScale);
    mat4x4 translationMatrix;
    mat4x4_identity(translationMatrix);
    switch(this->typeOfCentering){
        case FULL_CENTERED:
            mat4x4_translate_in_place(translationMatrix, this->position[0]-this->textWidth/2,this->position[1]-this->textHeight/2, 0.0f);
            break;
        case X_CENTERED:
            mat4x4_translate_in_place(translationMatrix, this->position[0]-this->textWidth/2,this->position[1], 0.0f);
            break;
        case Y_CENTERED:
            mat4x4_translate_in_place(translationMatrix, this->position[0],this->position[1]-this->textHeight/2, 0.0f);
            break;
        case TOP_CORNER:
            mat4x4_translate_in_place(translationMatrix, this->position[0],this->position[1]-this->textScale*0.7f, 0.0f);
            break;
        case TOP_CORNER_RIGHT:
            mat4x4_translate_in_place(translationMatrix, this->position[0]-this->textWidth,this->position[1]-this->textHeight, 0.0f);
            break;
    }
    mat4x4_mul(this->modelMatrix, translationMatrix, scaleMatrix);
}

void StringWithLimits::setScale(float pScale){
    this->textWidth=this->textWidth/this->textScale*pScale;
    this->textHeight=this->textHeight/this->textScale*pScale;
    this->textScale=pScale;
    this->setPosition(this->position);
}

StringWithLimits::StringWithLimits(std::string pText, float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, int pTypeOfCentering, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->typeOfCentering=pTypeOfCentering;


    this->maximumHeight=pMaximumHeight;
    if(this->maximumHeight==0){
        this->noLineSkipping=true;
    }
    this->maximumWidth=pMaximumWidth;

    mat4x4_identity(this->modelMatrix);

    this->textScale=pScale;

    this->modelMatrixLocation=glGetUniformLocation(*pShaderProgram,"M");
    //this->alphaLocation=glGetUniformLocation(*pShaderProgram,"alpha");

    this->vertices=(float*) malloc(sizeof(float)* pText.length()*6*3);
    this->textureCoords=(float*) malloc(sizeof(float)* pText.length()*6*2);

    unsigned width=1024, height=1024;

    this->textureHandle=fontTexture;

    GLuint TexLoc=glGetUniformLocation(*pShaderProgram,"myTextureSampler");
    if(TexLoc==(GLuint) -1){
        std::cout << "Couldn't locate textures!" << std::endl;
    }
    glUniform1i(TexLoc,0);


    std::ifstream fontDataFile("resources/myaheiui.fnt");
    std::string line;

    int k=0;

    while(std::getline(fontDataFile,line)){
        std::string tmp;
        std::stringstream iss(line);
        std::getline(iss, tmp, ' ');
        if(tmp.compare("char")==0){
            k++;
        }
    }

    fontDataFile.clear();
    fontDataFile.seekg(std::ios::beg);

    character myChar[k];

    k=0;
    while(std::getline(fontDataFile,line)){
        std::string tmp;
        std::stringstream iss(line);
        std::getline(iss, tmp, ' ');
        if(tmp.compare("char")==0){

            //ASCII code of the character
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].id=atoi(tmp.c_str());

            //X position in pixels (top left)
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].x=atoi(tmp.c_str());

            //Y position in pixels (top left)
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].y=atoi(tmp.c_str());

            //Width in pixels
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].width=atoi(tmp.c_str());

            //Height in Pixels
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].height=atoi(tmp.c_str());

            //Offset from cursor to the right
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].xoffset=atoi(tmp.c_str());

            //Offset from cursor to top left corner of next letter
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].yoffset=atoi(tmp.c_str());

            //Effective distance from letter to next cursor
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].xadvance=atoi(tmp.c_str());

            k++;
        }
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    float xtotal=0;
    float ytotal=0;
    this->textHeight=this->textScale*0.7;
    int lastSpaceIndex=-1;

    for(int i=0;i<(int) pText.length();i++){
        character tmpChar;
        for(int j=0;j<k;j++){
            if(myChar[j].id==pText[i]){
                tmpChar=myChar[j];
                break;
            }
        }

        if(tmpChar.id==' '){
            lastSpaceIndex=i;
        }

        int scale=160;
        float normalization=160.0f/scale;


        //Down left
        this->vertices[i*18+0]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+1]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+2]=0;

        //Top left
        this->vertices[i*18+3]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+4]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+5]=0;

        //Top right
        this->vertices[i*18+6]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+7]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+8]=0;

        // Top Right
        this->vertices[i*18+9]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+10]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+11]=0;

        //Down right
        this->vertices[i*18+12]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+13]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+14]=0;

        //Down left
        this->vertices[i*18+15]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+16]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+17]=0;

        this->textureCoords[i*12+0]=(float) tmpChar.x/width;
        this->textureCoords[i*12+1]=(float) tmpChar.y/height+(float)tmpChar.height/height;

        this->textureCoords[i*12+2]=(float) tmpChar.x/width;
        this->textureCoords[i*12+3]=(float) tmpChar.y/height;

        this->textureCoords[i*12+4]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+5]=(float) tmpChar.y/height;

        this->textureCoords[i*12+6]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+7]=(float) tmpChar.y/height;

        this->textureCoords[i*12+8]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+9]=(float) tmpChar.y/height+(float) tmpChar.height/height;

        this->textureCoords[i*12+10]=(float) tmpChar.x/width;
        this->textureCoords[i*12+11]=(float) tmpChar.y/height+(float) tmpChar.height/height;

        xtotal+=(float) tmpChar.xadvance/scale;
        if(xtotal*this->textScale>this->maximumWidth && lastSpaceIndex!=-1 && !this->noLineSkipping){
            ytotal-=1.1f;
            i=lastSpaceIndex; //Should be plus one, but i'll get augmented anyway
            this->textWidth=xtotal*this->textScale;
            xtotal=0;
        }
    }
    if(ytotal==0){
        this->textWidth=xtotal*this->textScale;
    }

    this->textHeight-=ytotal*this->textScale;

    this->setPosition(this->position);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pText.length()*6*3, vertices, GL_STATIC_DRAW);


    glGenBuffers(1,&this->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*pText.length()*6*2,this->textureCoords,GL_STATIC_DRAW);

    GLenum err;
    if( (err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }

    this->numberOfTriangles=pText.length()*6;
}

void StringWithLimits::setText(std::string pText){
    this->vertices=(float*) malloc(sizeof(float)* pText.length()*6*3);
    this->textureCoords=(float*) malloc(sizeof(float)* pText.length()*6*2);

    unsigned width=1024, height=1024;

    std::ifstream fontDataFile("resources/myaheiui.fnt");
    std::string line;

    int k=0;

    while(std::getline(fontDataFile,line)){
        std::string tmp;
        std::stringstream iss(line);
        std::getline(iss, tmp, ' ');
        if(tmp.compare("char")==0){
            k++;
        }
    }

    fontDataFile.clear();
    fontDataFile.seekg(std::ios::beg);

    character myChar[k];

    k=0;
    while(std::getline(fontDataFile,line)){
        std::string tmp;
        std::stringstream iss(line);
        std::getline(iss, tmp, ' ');
        if(tmp.compare("char")==0){

            //ASCII code of the character
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].id=atoi(tmp.c_str());

            //X position in pixels (top left)
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].x=atoi(tmp.c_str());

            //Y position in pixels (top left)
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].y=atoi(tmp.c_str());

            //Width in pixels
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].width=atoi(tmp.c_str());

            //Height in Pixels
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].height=atoi(tmp.c_str());

            //Offset from cursor to the right
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].xoffset=atoi(tmp.c_str());

            //Offset from cursor to top left corner of next letter
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].yoffset=atoi(tmp.c_str());

            //Effective distance from letter to next cursor
            std::getline(iss, tmp, '=');
            std::getline(iss, tmp, ' ');
            myChar[k].xadvance=atoi(tmp.c_str());

            k++;
        }
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    float xtotal=0;
    float ytotal=0;
    this->textHeight=this->textScale*0.7;
    int lastSpaceIndex=-1;

    for(int i=0;i<(int) pText.length();i++){
        character tmpChar;
        for(int j=0;j<k;j++){
            if(myChar[j].id==pText[i]){
                tmpChar=myChar[j];
                break;
            }
        }

        if(tmpChar.id==' '){
            lastSpaceIndex=i;
        }

        int scale=160;
        float normalization=160.0f/scale;


        //Down left
        this->vertices[i*18+0]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+1]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+2]=0;

        //Top left
        this->vertices[i*18+3]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+4]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+5]=0;

        //Top right
        this->vertices[i*18+6]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+7]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+8]=0;

        // Top Right
        this->vertices[i*18+9]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+10]=ytotal+normalization-(float) (tmpChar.yoffset)/scale;
        this->vertices[i*18+11]=0;

        //Down right
        this->vertices[i*18+12]=(float)tmpChar.width/scale+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+13]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+14]=0;

        //Down left
        this->vertices[i*18+15]=0+xtotal+(float) (tmpChar.xoffset)/scale;
        this->vertices[i*18+16]=ytotal+normalization-(float) (tmpChar.yoffset)/scale-(float) (tmpChar.height)/scale;
        this->vertices[i*18+17]=0;

        this->textureCoords[i*12+0]=(float) tmpChar.x/width;
        this->textureCoords[i*12+1]=(float) tmpChar.y/height+(float)tmpChar.height/height;

        this->textureCoords[i*12+2]=(float) tmpChar.x/width;
        this->textureCoords[i*12+3]=(float) tmpChar.y/height;

        this->textureCoords[i*12+4]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+5]=(float) tmpChar.y/height;

        this->textureCoords[i*12+6]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+7]=(float) tmpChar.y/height;

        this->textureCoords[i*12+8]=(float) tmpChar.x/width+(float)tmpChar.width/width;
        this->textureCoords[i*12+9]=(float) tmpChar.y/height+(float) tmpChar.height/height;

        this->textureCoords[i*12+10]=(float) tmpChar.x/width;
        this->textureCoords[i*12+11]=(float) tmpChar.y/height+(float) tmpChar.height/height;

        xtotal+=(float) tmpChar.xadvance/scale;

        if(xtotal*this->textScale>this->maximumWidth && lastSpaceIndex!=-1 && !this->noLineSkipping){
            ytotal-=1.1f;
            i=lastSpaceIndex;
            this->textWidth=xtotal*this->textScale;
            xtotal=0;
        }
    }
    if(ytotal==0){
        this->textWidth=xtotal*this->textScale;
    }

    this->textHeight-=ytotal*this->textScale;

    this->setPosition(this->position);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pText.length()*6*3, vertices, GL_STATIC_DRAW);


    glGenBuffers(1,&this->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*pText.length()*6*2,this->textureCoords,GL_STATIC_DRAW);

    GLenum err;
    if( (err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }

    this->numberOfTriangles=pText.length()*6;
}

void StringWithLimits::draw(float alpha){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->textureHandle);
    glUniformMatrix4fv(this->modelMatrixLocation,1,GL_FALSE,&this->modelMatrix[0][0]);
    glUniform1f(this->alphaLocation,alpha);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,this->vertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*) 0);

    glDrawArrays(GL_TRIANGLES,0,this->numberOfTriangles);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/**************************************************************
Color Box... Well, a simple Color Rectangle
**************************************************************/

ColorBox::ColorBox(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY, std::string texturePath, GLuint *pShaderProgram){
    this->position[0]=(upperRightX+lowerLeftX)/2;
    this->position[1]=(upperRightY+lowerLeftY)/2;

    this->height=(upperRightY-lowerLeftY);
    this->width=(upperRightX-lowerLeftX);

    this->alphaValue=0.7f;

    this->normalizedRatio=(this->height/2)/0.5;
    this->xyRatio=this->width/this->height;

    this->vertices = (float*) malloc(sizeof(float)*6*3);
    this->textureCoords = (float*) malloc(sizeof(float)*6*2);

    vertices=new float[18]{-0.5f,-0.5f,0.0f,
                           -0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,-0.5f,0.0f,
                           -0.5f,-0.5f,0.0f};

    textureCoords=new float[12]{0.0f,1.0f,
                               0.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,1.0f,
                               0.0f,1.0f};

    mat4x4_identity(this->modelMatrix);
    this->modelMatrixLocation=glGetUniformLocation(*pShaderProgram,"M");
    this->alphaLocation=glGetUniformLocation(*pShaderProgram,"alpha");

    this->setInstantPosition(upperRightX,upperRightY,lowerLeftX,lowerLeftY);

    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, texturePath);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

    glActiveTexture(GL_TEXTURE0);
    glGenTextures( 1, &this->textureHandle );
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, this->textureHandle );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    // build our texture mipmaps depending on system architecture

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    GLuint TexLoc=glGetUniformLocation(*pShaderProgram,"myTextureSampler");
    if(TexLoc==(GLuint) -1){
        std::cout << "Couldn't locate textures!" << std::endl;
    }
    glUniform1i(TexLoc,0);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3, vertices, GL_STATIC_DRAW);

    glGenBuffers(1,&this->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*2,this->textureCoords,GL_STATIC_DRAW);
}

void ColorBox::setInstantPosition(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY){
    this->position[0]=(upperRightX+lowerLeftX)/2;
    this->position[1]=(upperRightY+lowerLeftY)/2;

    this->height=(upperRightY-lowerLeftY);
    this->width=(upperRightX-lowerLeftX);

    this->normalizedRatio=(this->height/2)/0.5;
    this->xyRatio=this->width/this->height;

    float scale=this->normalizedRatio;
    mat4x4 translationMatrix;
    mat4x4_identity(translationMatrix);
    mat4x4_translate_in_place(translationMatrix, this->position[0],this->position[1],0.0f);
    mat4x4 scaleMatrix;
    mat4x4_identity(scaleMatrix);
    mat4x4_scale_aniso(scaleMatrix, scaleMatrix, this->xyRatio*scale,scale,1);

    mat4x4_mul(this->modelMatrix, translationMatrix, scaleMatrix);
}

void ColorBox::draw(float time){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->textureHandle);
    glUniformMatrix4fv(this->modelMatrixLocation,1,GL_FALSE,&this->modelMatrix[0][0]);
    glUniform1f(this->alphaLocation,this->alphaValue);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,this->vertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*) 0);

    //glDrawArrays(GL_TRIANGLE_FAN,0,21); Why the hell 21?
    glDrawArrays(GL_TRIANGLE_FAN,0,6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/*************************************************
TILE BOXXXXXXXXXXXXXXX
*************************************************/

ColorTile::ColorTile(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY, GLuint *pShaderProgram){
    this->position[0]=(upperRightX+lowerLeftX)/2;
    this->position[1]=(upperRightY+lowerLeftY)/2;

    this->height=(upperRightY-lowerLeftY);
    this->width=(upperRightX-lowerLeftX);

    this->alphaValue=0.6f;

    this->normalizedRatio=(this->height/2)/0.5;
    this->xyRatio=this->width/this->height;

    this->vertices = (float*) malloc(sizeof(float)*6*3);
    this->textureCoords = (float*) malloc(sizeof(float)*6*2);

    vertices=new float[18]{-0.5f,-0.5f,0.0f,
                           -0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,-0.5f,0.0f,
                           -0.5f,-0.5f,0.0f};

    textureCoords=new float[12]{0.0f,1.0f,
                               0.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,1.0f,
                               0.0f,1.0f};

    mat4x4_identity(this->modelMatrix);

    this->modelMatrixLocation=glGetUniformLocation(*pShaderProgram,"M");
    //this->alphaLocation=glGetUniformLocation(*pShaderProgram,"alpha");

    this->setInstantPosition(upperRightX,upperRightY,lowerLeftX,lowerLeftY);

    this->textureHandle=fontTexture;

    GLuint TexLoc=glGetUniformLocation(*pShaderProgram,"myTextureSampler");
    if(TexLoc==(GLuint) -1){
        std::cout << "Couldn't locate textures!" << std::endl;
    }
    glUniform1i(TexLoc,0);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3, vertices, GL_STATIC_DRAW);

    glGenBuffers(1,&this->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*2,this->textureCoords,GL_STATIC_DRAW);
}

void ColorTile::setInstantPosition(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY){
    this->position[0]=(upperRightX+lowerLeftX)/2;
    this->position[1]=(upperRightY+lowerLeftY)/2;

    this->height=(upperRightY-lowerLeftY);
    this->width=(upperRightX-lowerLeftX);

    this->normalizedRatio=(this->height/2)/0.5;
    this->xyRatio=this->width/this->height;

    float scale=this->normalizedRatio;
    mat4x4 translationMatrix;
    mat4x4_identity(translationMatrix);
    mat4x4_translate_in_place(translationMatrix,this->position[0],this->position[1],0.0f);
    mat4x4 scaleMatrix;
    mat4x4_identity(scaleMatrix);
    mat4x4_scale_aniso(scaleMatrix, scaleMatrix, this->xyRatio*scale,scale,1);

    mat4x4_mul(modelMatrix, translationMatrix, scaleMatrix);
}

void ColorTile::draw(float time){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->textureHandle);
    glUniformMatrix4fv(this->modelMatrixLocation,1,GL_FALSE,&this->modelMatrix[0][0]);
    //glUniform1f(this->alphaLocation,this->alphaValue);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,this->vertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*) 0);

    //glDrawArrays(GL_TRIANGLE_FAN,0,21); Why the hell 21?
    glDrawArrays(GL_TRIANGLE_FAN,0,6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void ColorTile::setAttended(){
    this->textureHandle=fontTexture;
}

/**************************************************************
Tick Box!!!!!!!!!!!! It's the shits
**************************************************************/

TickBox::TickBox(float pPosition[2], float pScale, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];

    this->scale=pScale;

    this->vertices = (float*) malloc(sizeof(float)*6*3);
    this->textureCoords = (float*) malloc(sizeof(float)*6*2);

    vertices=new float[18]{-0.5f,-0.5f,0.0f,
                           -0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,0.5f,0.0f,
                           0.5f,-0.5f,0.0f,
                           -0.5f,-0.5f,0.0f};

    textureCoords=new float[12]{0.0f,1.0f,
                               0.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,0.0f,
                               1.0f,1.0f,
                               0.0f,1.0f};

    mat4x4_identity(this->modelMatrix);

    this->modelMatrixLocation=glGetUniformLocation(*pShaderProgram,"M");
    //this->alphaLocation=glGetUniformLocation(*pShaderProgram,"alpha");

    this->setInstantPosition(this->position);

    this->textureHandle=fontTexture;
    this->isTicked=true;

    GLuint TexLoc=glGetUniformLocation(*pShaderProgram,"myTextureSampler");
    if(TexLoc==(GLuint) -1){
        std::cout << "Couldn't locate textures!" << std::endl;
    }
    glUniform1i(TexLoc,0);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3, vertices, GL_STATIC_DRAW);

    glGenBuffers(1,&this->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*2,this->textureCoords,GL_STATIC_DRAW);
}

void TickBox::setInstantPosition(float pPosition[2]){

    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];

    mat4x4 translationMatrix;
    mat4x4_identity(translationMatrix);
    mat4x4_translate_in_place(translationMatrix, pPosition[0],pPosition[1],0.0f);
    mat4x4 scaleMatrix;
    mat4x4_identity(scaleMatrix);
    mat4x4_scale_aniso(scaleMatrix, scaleMatrix, this->scale,this->scale,1);

    mat4x4_mul(this->modelMatrix, translationMatrix, scaleMatrix);
}

void TickBox::draw(float time){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->textureHandle);
    glUniformMatrix4fv(this->modelMatrixLocation,1,GL_FALSE,&this->modelMatrix[0][0]);
    glUniform1f(this->alphaLocation,1);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,this->vertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,this->textureBuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*) 0);

    //glDrawArrays(GL_TRIANGLE_FAN,0,21); Why the hell 21?
    glDrawArrays(GL_TRIANGLE_FAN,0,6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void TickBox::toggleState(void){
    /*
    this->isTicked=!this->isTicked;
    if(this->isTicked){
        this->textureHandle=checkedTexture;
    } else {
        this->textureHandle=uncheckedTexture;
    }
    */
}
void TickBox::setState(bool state){
    /*
    this->isTicked=state;
    if(this->isTicked){
        this->textureHandle=checkedTexture;
    } else {
        this->textureHandle=uncheckedTexture;
    }
    */
}

/***************************************************************
Edit box stuff
***************************************************************/

EditBox::EditBox(std::string helpText, int pMaxWidth, float pPosition[2], float pScale, int typeOfCentering, GLuint *pShaderProgram){
    float editPosition[2]={pPosition[0],pPosition[1]-pScale*0.2f};
    this->myStringHelper= new StringWithLimits(helpText,editPosition,pScale,pMaxWidth,10,typeOfCentering,pShaderProgram);
    editPosition[0]+=this->myStringHelper->getWidth();
    this->editedText= new StringWithLimits("",editPosition,pScale,pMaxWidth-this->myStringHelper->getWidth(),10,typeOfCentering,pShaderProgram);
    this->boxShowing=true;

    this->surroundingBox= new ColorBox(pPosition[0],pPosition[1],pPosition[0]+pMaxWidth,pPosition[1]-pScale*0.4f-this->myStringHelper->getHeight(),"resources/box.png",pShaderProgram);
}

void EditBox::draw(float time){
    if(boxShowing){
        this->surroundingBox->draw(time);
    }
    this->myStringHelper->draw(1);
    this->editedText->draw(1);
}
void EditBox::insertCharacter(char nextChar){
    this->text+=nextChar;
    this->editedText->setText(this->text);
}
void EditBox::removeCharacter(void){
    if(text.length()>0){
        this->text=this->text.substr(0,text.size()-1);
        this->editedText->setText(this->text);
    }
}
void EditBox::clearText(void){
    this->text="";
    this->editedText->setText(this->text);
}


/****************************************************************
Notification Displayer functions
****************************************************************/

NotificationDisplayer::NotificationDisplayer(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, float pAnimationTimes[3],GLuint *pShaderProgram){
    this->isDisplaying=false;
    this->animationTimes[0]=pAnimationTimes[0];
    this->animationTimes[1]=pAnimationTimes[1];
    this->animationTimes[2]=pAnimationTimes[2];
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->myString = new StringWithLimits("",pPosition,pScale,pMaximumWidth,pMaximumHeight,StringWithLimits::FULL_CENTERED,pShaderProgram);
}

void NotificationDisplayer::pushNotification(std::string notificationText){
    this->notificationStrings.push(notificationText);
}

void NotificationDisplayer::draw(float time){
    if(!this->isDisplaying){
        if(!this->notificationStrings.empty()){
            std::string tmpString = this->notificationStrings.front();
            this->myString->setText(tmpString);
            this->notificationStrings.pop();
            this->initTime=time;
            this->isDisplaying=true;
        }
    } else {
        float dt=time-initTime;
        if(dt<this->animationTimes[0]){
            float alpha=(1-cos(dt*M_PI/this->animationTimes[0]))/2;
            this->myString->draw(alpha);
        } else if(dt < this->animationTimes[1] + this->animationTimes[0] ){
            this->myString->draw(1);
        } else if(dt < this->animationTimes[2] + this->animationTimes[1] + this->animationTimes[0] ){
            dt-=this->animationTimes[1]+this->animationTimes[0];
            float alpha=(1+cos(dt*M_PI/this->animationTimes[2]))/2;
            this->myString->draw(alpha);
        } else {
            this->isDisplaying=false;
        }
    }
}
