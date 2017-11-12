#ifndef INCLUDE_TXTRESOURCES_H
#define INCLUDE_TXTRESOURCES_H

#ifdef _WIN32
    #define APIENTRY __stdcall
#endif
#include "glad/glad.h"

#include<GLFW/glfw3.h>
#include<string>
#define M_PI 3.141592
#include<vector>
#include<queue>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>

#include"brate/lodepng.h"
#include"brate/stringfix.h"
#include "brate/linmath.h"

extern GLuint fontTexture;

void StartFontTexture(void);

class StringWithLimits{
private:
    //Variables regarding the physical aspects of the string
    float position[2];
    float maximumWidth;
    float maximumHeight;
    float textWidth;
    float textHeight;
    float textScale;
    int typeOfCentering;
    std::string text;
    bool noLineSkipping;

    //Variables for OpenGL
    GLuint *shaderProgram;
    GLuint modelMatrixLocation;
    GLuint alphaLocation;
    GLuint textureHandle;
    GLuint vertexBuffer;
    GLuint textureBuffer;
    int numberOfTriangles;
    mat4x4 modelMatrix;
    float *vertices;
    float *textureCoords;
public:
    static const int FULL_CENTERED=0;
    static const int X_CENTERED=1;
    static const int Y_CENTERED=2;
    static const int TOP_CORNER=3;
    static const int TOP_CORNER_RIGHT=4;

    void setPosition(float pPosition[2]);

    StringWithLimits(std::string pText, float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, int pTypeOfCentering, GLuint *pShaderProgram);

    //Modifiers for the string
    void setText(std::string pText);
    void setScale(float pScale);

    //Getters
    float getWidth(void){ return this->textWidth;};
    float getHeight(void){ return this->textHeight;};
    float getXPos(void){ return this->position[0];};
    float getYPos(void){ return this->position[1];};
    float getScale(void){ return this->textScale;};

    //The draw function
    void draw(float alpha);
};

class ColorBox{
private:
    float position[2];
    float height;
    float width;
    float normalizedRatio;
    float xyRatio;
    std::string textureFileName;
    float alphaValue;

    //OpenGL stuff for assholes
    float *vertices;
    float *textureCoords;
    mat4x4 modelMatrix;
    GLuint modelMatrixLocation,alphaLocation,textureHandle,vertexBuffer,textureBuffer;
    bool firstDraw;
    bool moveAnimation;
    double initializationTime;
    void checkAnimation(double time);
public:
    ColorBox(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY, std::string texturePath, GLuint *pShaderProgram);
    void setInstantPosition(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY);
    void setAlphaValue(float a){this->alphaValue=a;};
    void draw(float time);
};

class ColorTile{
private:
    float position[2];
    float height;
    float width;
    float normalizedRatio;
    float xyRatio;
    std::string textureFileName;
    float alphaValue;

    //OpenGL stuff for assholes
    float *vertices;
    float *textureCoords;
    mat4x4 modelMatrix;
    GLuint modelMatrixLocation,alphaLocation,textureHandle,vertexBuffer,textureBuffer;
    bool firstDraw;
    bool moveAnimation;
    double initializationTime;
    void checkAnimation(double time);
public:
    ColorTile(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY, GLuint *pShaderProgram);
    void setInstantPosition(float upperRightX, float upperRightY, float lowerLeftX, float lowerLeftY);
    void setAlphaValue(float a){this->alphaValue=a;};
    void setAttended(void);
    void draw(float time);
};

class EditBox{
private:
    float position[2];
    float maxWidth;
    float scale;
    GLuint *shaderProgram;
    bool boxShowing;

    ColorBox *surroundingBox;
    StringWithLimits *myStringHelper;
    StringWithLimits *editedText;
    std::string text;
public:
    EditBox(std::string helpText, int pMaxWidth, float pPosition[2], float pScale, int typeOfCentering, GLuint *pShaderProgram);
    void draw(float time);

    void turnOffBox(void){this->boxShowing=false;};

    void insertCharacter(char nextChar);
    void removeCharacter(void);
    void clearText(void);
    std::string getText(void){ return this->text;};
};

class NotificationDisplayer{
private:
    GLuint *shaderProgram;
    //This thing contains: Fade in time, show time, fade out time
    float animationTimes[3];

    float position[2];
    bool isDisplaying;

    std::queue<std::string> notificationStrings;

    StringWithLimits *myString;

    //Auxiliary variables
    float initTime;

public:
    NotificationDisplayer(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, float pAnimationTimes[3],GLuint *pShaderProgram);

    void pushNotification(std::string notificationText);

    void draw(float time);
};

class character{
public:
    character(void){};
    char id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
};

class TickBox{
private:
    float position[2];
    float scale;
    bool isTicked;

    //OpenGL stuff for assholes
    float *vertices;
    float *textureCoords;
    mat4x4 modelMatrix;
    GLuint modelMatrixLocation,alphaLocation,textureHandle,vertexBuffer,textureBuffer;
public:
    TickBox(float pPosition[2], float pScale, GLuint *pShaderProgram);
    void setInstantPosition(float pPosition[2]);
    bool getIfTicked(void){return this->isTicked;};
    void toggleState(void);
    void setState(bool state);
    void draw(float time);
};

/***********************************************************************************
Finally, I'll move the next part to the bottom as they are heavily templated classes
***********************************************************************************/

template<class T>
class ListOfStrings{
private:
    float position[2];
    float maximumWidth;
    float maximumHeight;
    float scale;
    int selectedOption;

    GLuint *shaderProgram;

    std::vector<StringWithLimits> options;
    std::vector<T> elements;

    ColorBox *myMargin;
    ColorBox *myOptionSelector;
public:
    ListOfStrings(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram);
    void emptyList(void);
    void pushString(std::string option, T element);

    void goUp(void);
    void goDown(void);
    int getID(void);
    void flush(void);
    float getXPos(void){ return this->position[0];};
    float getYPos(void){ return this->position[1];};
    int checkIfClicked(float xpos, float ypos);
    bool isEmpty(void){ return this->options.empty();};
    int getNumElements(void){ return this->options.size();};
    T elementAt(int i){ return this->elements.at(i);};

    void draw(float time);
};

template<class T>
ListOfStrings<T>::ListOfStrings(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->scale=pScale;
    this->shaderProgram=pShaderProgram;
    this->maximumWidth=0;
    this->myMargin = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/box.png",pShaderProgram);
    this->myOptionSelector = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/selection.png",pShaderProgram);
    this->myOptionSelector->setAlphaValue(0.9f);
}

template<class T>
void ListOfStrings<T>::pushString(std::string option, T element){
    float tmpPosition[2];
    if(this->options.empty()){
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->position[1];
    } else {
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->options.back().getYPos()-this->options.back().getScale();
    }
    this->options.push_back(*(new StringWithLimits(option,tmpPosition,this->scale,5,10,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    if(this->maximumWidth < this->options.back().getWidth()){
        this->maximumWidth=this->options.back().getWidth();
    }
    this->elements.push_back(element);

    this->myMargin->setInstantPosition(this->position[0],
                                       this->position[1]+this->options.back().getHeight()*0.2f,
                                       this->position[0]+this->maximumWidth,
                                       this->options.back().getYPos()-this->options.back().getHeight()*1.2f);
    this->myOptionSelector->setInstantPosition(this->position[0],
                                               this->position[1]+this->options.back().getHeight()*0.2f,
                                               this->position[0]+this->maximumWidth,
                                               this->position[1]-this->options.back().getHeight()*1.2f);
    this->selectedOption=0;

}

template<class T>
void ListOfStrings<T>::goUp(void){
    if(this->selectedOption>0 && !this->options.empty()){
        this->selectedOption--;
        this->myOptionSelector->setInstantPosition(this->options.at(this->selectedOption).getXPos(),
                                                   this->options.at(this->selectedOption).getYPos()+this->options.back().getHeight()*0.2f,
                                                   this->options.at(this->selectedOption).getXPos()+this->maximumWidth,
                                                   this->options.at(this->selectedOption).getYPos()-this->options.back().getHeight()*1.2f);
    }
}

template<class T>
void ListOfStrings<T>::goDown(void){
    if(this->selectedOption<(this->options.size()-1) && !this->options.empty()){
        this->selectedOption++;
        this->myOptionSelector->setInstantPosition(this->options.at(this->selectedOption).getXPos(),
                                                   this->options.at(this->selectedOption).getYPos()+this->options.back().getHeight()*0.2f,
                                                   this->options.at(this->selectedOption).getXPos()+this->maximumWidth,
                                                   this->options.at(this->selectedOption).getYPos()-this->options.back().getHeight()*1.2f);
    }
}

template<class T>
int ListOfStrings<T>::getID(void){
    return this->selectedOption;
}

template<class T>
void ListOfStrings<T>::flush(void){
    this->options.clear();
    this->elements.clear();
}

template<class T>
int ListOfStrings<T>::checkIfClicked(float xpos, float ypos){
    if(xpos> this->position[0] && xpos < this->maximumWidth+this->position[0]){
        for(int i=0;i<this->options.size(); i++){
            if(ypos<(this->options.at(i).getYPos()+this->options.at(i).getHeight()*0.2f) && ypos > (this->options.at(i).getYPos()-this->options.at(i).getHeight()*1.2f)){
                return i;
            }
        }
    }
    return -1;
}

template<class T>
void ListOfStrings<T>::draw(float time){
    if(!this->options.empty()){
        this->myMargin->draw(time);
        this->myOptionSelector->draw(time);
        for(int i=0;i<this->options.size();i++){
            this->options.at(i).draw(1);
        }
    }
}

/************
For the checkable list of strings
CHECK THAT BY DEFAULT THIS THING ALLOWS MANY TICKS PER LIST
************/

template<class T>
class CheckableListOfStrings{
private:
    float position[2];
    float maximumWidth;
    float maximumHeight;
    float scale;
    int selectedOption;
    bool uniqueSelection;

    GLuint *shaderProgram;

    std::vector<StringWithLimits> options;
    std::vector<T> elements;
    std::vector<TickBox> myTickBoxes;

    ColorBox *myMargin;
    ColorBox *myOptionSelector;
public:
    CheckableListOfStrings(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram);
    void pushString(std::string option, T element);
    std::vector<T> getSelectedElements(void);

    void goUp(void);
    void goDown(void);
    int setUniqueSelection(void);
    int unsetUniqueSelection(void);
    int getNumElements(void){ return this->options.size();};
    bool isEmpty(void){ return this->options.empty();};
    void toggleCurrent(void);
    void flush(void);
    float getXPos(void){ return this->position[0];};
    float getYPos(void){ return this->position[1];};
    int checkIfClicked(float xpos, float ypos);

    void draw(float time);
};

template<class T>
CheckableListOfStrings<T>::CheckableListOfStrings(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->uniqueSelection=false;
    this->scale=pScale;
    this->shaderProgram=pShaderProgram;
    this->maximumWidth=0;
    this->myMargin = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/box.png",pShaderProgram);
    this->myOptionSelector = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/selection.png",pShaderProgram);
    this->myOptionSelector->setAlphaValue(0.9f);
}

template<class T>
void CheckableListOfStrings<T>::pushString(std::string option, T element){
    float tmpPosition[2];
    if(this->options.empty()){
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->position[1];
    } else {
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->options.back().getYPos()-this->options.back().getScale();
    }
    this->options.push_back(*(new StringWithLimits(option,tmpPosition,this->scale,5,10,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    if(this->maximumWidth < this->options.back().getWidth()){
        this->maximumWidth=this->options.back().getWidth();
    }
    this->elements.push_back(element);

    tmpPosition[0]-=this->scale*0.8f/2;
    tmpPosition[1]-=this->scale*0.8f/2;
    this->myTickBoxes.push_back(*(new TickBox(tmpPosition,this->scale*0.8f,this->shaderProgram)));

    if(this->uniqueSelection && this->myTickBoxes.size()>1){
        this->myTickBoxes.back().toggleState();
    }

    this->myMargin->setInstantPosition(this->position[0],
                                       this->position[1]+this->options.back().getHeight()*0.2f,
                                       this->position[0]+this->maximumWidth,
                                       this->options.back().getYPos()-this->options.back().getHeight()*1.2f);
    this->myOptionSelector->setInstantPosition(this->position[0],
                                               this->position[1]+this->options.back().getHeight()*0.2f,
                                               this->position[0]+this->maximumWidth,
                                               this->position[1]-this->options.back().getHeight()*1.2f);
    this->selectedOption=0;

}

template<class T>
std::vector<T> CheckableListOfStrings<T>::getSelectedElements(void){
    std::vector<T> tmpVect;
    for(int i=0;i<this->elements.size();i++){
        if(this->myTickBoxes.at(i).getIfTicked()){
            tmpVect.push_back(this->elements.at(i));
        }
    }
    return tmpVect;
}

template<class T>
void CheckableListOfStrings<T>::goUp(void){
    if(this->selectedOption>0 && !this->options.empty()){
        this->selectedOption--;
        this->myOptionSelector->setInstantPosition(this->options.at(this->selectedOption).getXPos(),
                                                   this->options.at(this->selectedOption).getYPos()+this->options.back().getHeight()*0.2f,
                                                   this->options.at(this->selectedOption).getXPos()+this->maximumWidth,
                                                   this->options.at(this->selectedOption).getYPos()-this->options.back().getHeight()*1.2f);
    }
}

template<class T>
void CheckableListOfStrings<T>::goDown(void){
    if(this->selectedOption<(this->options.size()-1) && !this->options.empty()){
        this->selectedOption++;
        this->myOptionSelector->setInstantPosition(this->options.at(this->selectedOption).getXPos(),
                                                   this->options.at(this->selectedOption).getYPos()+this->options.back().getHeight()*0.2f,
                                                   this->options.at(this->selectedOption).getXPos()+this->maximumWidth,
                                                   this->options.at(this->selectedOption).getYPos()-this->options.back().getHeight()*1.2f);
    }
}

template<class T>
int CheckableListOfStrings<T>::setUniqueSelection(void){
    this->uniqueSelection=true;
}

template<class T>
int CheckableListOfStrings<T>::unsetUniqueSelection(void){
    this->uniqueSelection=false;
}

template<class T>
void CheckableListOfStrings<T>::toggleCurrent(void){
    if(!this->uniqueSelection){
        this->myTickBoxes.at(this->selectedOption).toggleState();
    } else {
        for(int i=0;i<this->myTickBoxes.size();i++){
            if(i==this->selectedOption){
                if(this->myTickBoxes.at(i).getIfTicked()){
                    this->myTickBoxes.at(i).setState(false);
                } else {
                    this->myTickBoxes.at(i).setState(true);
                }
            } else {
                this->myTickBoxes.at(i).setState(false);
            }
        }
    }
}

template<class T>
void CheckableListOfStrings<T>::flush(void){
    this->options.clear();
    this->myTickBoxes.clear();
    this->elements.clear();
}

template<class T>
int CheckableListOfStrings<T>::checkIfClicked(float xpos, float ypos){
    if(xpos> this->position[0] && xpos < this->maximumWidth+this->position[0]){
        for(int i=0;i<this->options.size(); i++){
            if(ypos<(this->options.at(i).getYPos()+this->options.at(i).getHeight()*0.2f) && ypos > (this->options.at(i).getYPos()-this->options.at(i).getHeight()*1.2f)){
                return i;
            }
        }
    }
    return -1;
}

template<class T>
void CheckableListOfStrings<T>::draw(float time){
    if(!this->options.empty()){
        this->myMargin->draw(time);
        this->myOptionSelector->draw(time);
        for(int i=0;i<this->options.size();i++){
            this->options.at(i).draw(1);
        }

        for(int i=0;i<this->myTickBoxes.size();i++){
            this->myTickBoxes.at(i).draw(time);
        }
    }
}

/*****
For multiple detailed List
*****/

template<class T>
class MultipleDetailsList{
private:
    float position[2];
    float maximumWidth;
    float maximumHeight;
    float scale;
    int selectedOption;
    int maxElementsPerScreen;

    GLuint *shaderProgram;

    std::vector<std::vector<StringWithLimits> > options;
    std::vector<T> elements;

    StringWithLimits *pageNumbering;

    ColorBox *myMargin;
    ColorBox *myOptionSelector;
public:
    MultipleDetailsList(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram);
    void emptyList(void);
    void pushOption(std::string op1, std::string op2, std::string op3, T element);

    void goUp(void);
    void goDown(void);
    int getID(void);
    T getSelectedElement(void);
    void flush(void);
    float getXPos(void){ return this->position[0];};
    float getYPos(void){ return this->position[1];};
    int checkIfClicked(float xpos, float ypos);

    void draw(float time);
};

template<class T>
MultipleDetailsList<T>::MultipleDetailsList(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->scale=pScale;
    this->shaderProgram=pShaderProgram;
    this->maximumWidth=pMaximumWidth;
    this->maximumHeight=pMaximumHeight;
    this->myMargin = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/box.png",pShaderProgram);
    this->myOptionSelector = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/selection.png",pShaderProgram);
    this->myOptionSelector->setAlphaValue(0.9f);
    this->maxElementsPerScreen = (int) floor(this->maximumHeight/(1.7*this->scale));

    float tmpPosition[]={this->position[0]+this->maximumWidth/2,this->position[1]-this->maximumHeight};
    this->pageNumbering = new StringWithLimits("1 de 1",tmpPosition,this->scale*0.9f,10,10,StringWithLimits::X_CENTERED,this->shaderProgram);
}

template<class T>
void MultipleDetailsList<T>::pushOption(std::string op1, std::string op2, std::string op3, T element){
    float tmpPosition[2];
    if(this->options.empty() || this->options.size()%this->maxElementsPerScreen==0){
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->position[1];
    } else {
        tmpPosition[0]=this->position[0];
        tmpPosition[1]=this->options.back().back().getYPos()-this->options.back().back().getHeight()-this->options.back().back().getScale()*0.4f;
    }
    std::vector<StringWithLimits> tmpSWL;
    tmpSWL.push_back(*(new StringWithLimits(op1,tmpPosition,this->scale,5,10,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    tmpPosition[0]+=this->maximumWidth;
    tmpSWL.push_back(*(new StringWithLimits(op2,tmpPosition,this->scale,5,10,StringWithLimits::TOP_CORNER_RIGHT,this->shaderProgram)));
    tmpPosition[0]-=this->maximumWidth;
    tmpPosition[1]-=tmpSWL.front().getHeight()*1.5f;
    tmpSWL.push_back(*(new StringWithLimits(op3,tmpPosition,this->scale*0.5f,this->maximumWidth,10,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    this->options.push_back(tmpSWL);
    this->elements.push_back(element);

    /**
    this->myMargin->setInstantPosition(this->position[0],
                                       this->position[1]+this->options.front().front().getHeight()*0.2f,
                                       this->position[0]+this->maximumWidth,
                                       this->options.back().back().getYPos()-this->options.back().back().getHeight()-this->options.back().back().getScale()*0.4f);
                                       **/
    this->myMargin->setInstantPosition(this->position[0],
                                       this->position[1],
                                       this->position[0]+this->maximumWidth,
                                       this->position[1]-this->maximumHeight);

    this->myOptionSelector->setInstantPosition(this->position[0],
                                               this->position[1]+this->options.front().front().getHeight()*0.2f,
                                               this->position[0]+this->maximumWidth,
                                               this->options.front().back().getYPos()-this->options.front().back().getHeight()-this->options.front().back().getScale()*0.4f);
    this->selectedOption=0;

    int etape=(this->selectedOption/this->maxElementsPerScreen);
    this->pageNumbering->setText(patch::to_string(etape+1)+" de "+patch::to_string(this->options.size()/this->maxElementsPerScreen+1));

}

template<class T>
void MultipleDetailsList<T>::goUp(void){
    if(this->selectedOption>0 && !this->options.empty()){
        this->selectedOption--;
        this->myOptionSelector->setInstantPosition(this->position[0],
                                                   this->options.at(this->selectedOption).front().getYPos()+this->options.at(this->selectedOption).front().getHeight()*0.2f,
                                                   this->position[0]+this->maximumWidth,
                                                   this->options.at(this->selectedOption).back().getYPos()-this->options.at(this->selectedOption).back().getHeight()-this->options.at(this->selectedOption).back().getScale()*0.4f);
    }

    int etape=(this->selectedOption/this->maxElementsPerScreen);
    this->pageNumbering->setText(patch::to_string(etape+1)+" de "+patch::to_string(this->options.size()/this->maxElementsPerScreen+1));
}

template<class T>
void MultipleDetailsList<T>::goDown(void){
    if(this->selectedOption<(this->options.size()-1) && !this->options.empty()){
        this->selectedOption++;
        this->myOptionSelector->setInstantPosition(this->position[0],
                                                   this->options.at(this->selectedOption).front().getYPos()+this->options.back().back().getHeight()*0.2f,
                                                   this->position[0]+this->maximumWidth,
                                                   this->options.at(this->selectedOption).back().getYPos()-this->options.at(this->selectedOption).back().getHeight()-this->options.at(this->selectedOption).back().getScale()*0.4f);

        int etape=(this->selectedOption/this->maxElementsPerScreen);
        this->pageNumbering->setText(patch::to_string(etape+1)+" de "+patch::to_string(this->options.size()/this->maxElementsPerScreen+1));
    }
}

template<class T>
int MultipleDetailsList<T>::getID(void){
    return this->selectedOption;
}

template<class T>
T MultipleDetailsList<T>::getSelectedElement(void){
    return this->elements.at(this->selectedOption);
}

template<class T>
void MultipleDetailsList<T>::flush(void){
    this->options.clear();
    this->elements.clear();
}

template<class T>
int MultipleDetailsList<T>::checkIfClicked(float xpos, float ypos){
    if(xpos> this->position[0] && xpos < this->maximumWidth+this->position[0]){
        for(int i=0;i<this->options.size(); i++){
            if(ypos<(this->options.at(i).front().getYPos()+this->options.at(i).front().getHeight()*0.2f) && ypos > (this->options.at(i).back().getYPos()-this->options.at(i).back().getHeight()*1.2f)){
                return i;
            }
        }
    }
    return -1;
}

template<class T>
void MultipleDetailsList<T>::draw(float time){
    if(!this->options.empty()){
        int etape=(this->selectedOption/this->maxElementsPerScreen) * this->maxElementsPerScreen;
        this->myMargin->draw(time);
        this->myOptionSelector->draw(time);
        this->pageNumbering->draw(1);
        for(int i=etape;i<this->options.size() && i<etape+this->maxElementsPerScreen;i++){
            for(int j=0;j<this->options.at(i).size();j++){
                this->options.at(i).at(j).draw(1);
            }
        }
    }
}

/**************************************************************************************
For Completely detailed list
**************************************************************************************/

template<class T>
class CompleteDetailsList{
private:
    float position[2];
    float maximumWidth;
    float maximumHeight;
    float scale;
    int selectedOption;
    int maxElementsPerScreen;

    GLuint *shaderProgram;

    std::vector<std::vector<StringWithLimits> > options;
    std::vector<T> elements;

    ColorBox *myOptionSelector;
public:
    class StoringArray{
    private:
        int state;
    public:
        static const int UNATTENDED=0;
        static const int ATTENDED=1;
        static const int FINISHED=2;
        std::string name;
        std::string garnish;
        std::vector<std::string> toppings;
        std::string who;
        T element;
        StoringArray(std::string pName, std::string pGarnish, std::vector<std::string> pToppings, std::string pWho, T pElement){
            this->name=pName; this->garnish=pGarnish; this->toppings=pToppings; this->who=pWho; this->element=pElement;
            state=UNATTENDED;
        }
        int getState(void){ return this->state;};
        void advanceState(void){ this->state++;};
    };

    CompleteDetailsList(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram);
    void emptyList(void);
    void pushOption(std::string name, std::string garnish, std::vector<std::string> toppings, std::string who, T element);

    void goUp(void);
    void goDown(void);
    int getID(void);
    T getSelectedElement(void);
    void flush(void);
    float getXPos(void){ return this->position[0];};
    float getYPos(void){ return this->position[1];};
    int checkIfClicked(float xpos, float ypos);

    int killClicked(int whoToKill);

    void draw(float time);

private:
    std::queue<StoringArray> nextOrders;
    std::vector<StoringArray> currentOrders;
    std::vector<ColorTile> myTiles;
    void loadTile(StoringArray SAT);
};

template<class T>
CompleteDetailsList<T>::CompleteDetailsList(float pPosition[2], float pScale, float pMaximumWidth, float pMaximumHeight, GLuint *pShaderProgram){
    this->position[0]=pPosition[0];
    this->position[1]=pPosition[1];
    this->scale=pScale;
    this->shaderProgram=pShaderProgram;
    this->maximumWidth=pMaximumWidth;
    this->maximumHeight=pMaximumHeight;
    this->myOptionSelector = new ColorBox(pPosition[0],pPosition[1],pPosition[0],pPosition[1],"res/selection.png",pShaderProgram);
    this->myOptionSelector->setAlphaValue(0.9f);
    this->maxElementsPerScreen = (int) floor(this->maximumHeight/(1.7*this->scale));

    float tmpPosition[]={this->position[0]+this->maximumWidth/2,this->position[1]-this->maximumHeight};
}

template<class T>
void CompleteDetailsList<T>::pushOption(std::string name, std::string garnish, std::vector<std::string> toppings, std::string who, T element){
    if(this->options.size()<8){
        StoringArray tmp=*(new StoringArray(name,garnish,toppings,who,element));
        loadTile(tmp);
        this->currentOrders.push_back(tmp);
    } else {
        this->nextOrders.push(*(new StoringArray(name,garnish,toppings,who,element)));
    }
}

template<class T>
void CompleteDetailsList<T>::loadTile(StoringArray SAT){
    float tmpPosition[2];
    tmpPosition[0]=this->position[0]+this->options.size()%4*(this->maximumWidth/4);
    tmpPosition[1]=this->position[1]-(this->options.size()/4)%2*(this->maximumHeight/2);

    this->myTiles.push_back(*(new ColorTile(tmpPosition[0]+this->maximumWidth/4,tmpPosition[1],tmpPosition[0],tmpPosition[1]-this->maximumHeight/2, this->shaderProgram)));
    if(SAT.getState()==StoringArray::ATTENDED){
        this->myTiles.back().setAttended();
    }

    tmpPosition[0]+=this->maximumWidth/4*0.05f;
    tmpPosition[1]-=this->scale*0.5f;

    std::vector<StringWithLimits> tmpSWL;
    tmpSWL.push_back(*(new StringWithLimits(SAT.name,tmpPosition,this->scale,this->maximumWidth/4*0.9f,this->maximumHeight,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    tmpPosition[1]-=tmpSWL.back().getHeight()+this->scale*0.5f;
    tmpSWL.push_back(*(new StringWithLimits(std::string("G: ")+SAT.garnish,tmpPosition,0.6f*this->scale,this->maximumWidth/4*0.9f,this->maximumHeight,StringWithLimits::TOP_CORNER,this->shaderProgram)));
    tmpPosition[1]-=tmpSWL.back().getHeight()+this->scale*0.5f;
    for(int i=0;i<SAT.toppings.size();i++){
        tmpSWL.push_back(*(new StringWithLimits(SAT.toppings.at(i),tmpPosition,this->scale*0.6f,this->maximumWidth/4*0.9f,this->maximumHeight,StringWithLimits::TOP_CORNER,this->shaderProgram)));
        tmpPosition[1]-=tmpSWL.back().getHeight()*1.4f;
    }
    tmpPosition[1]=this->position[1]-this->maximumHeight/2+this->scale*1.2f-(this->options.size()/4)%2*(this->maximumHeight/2);
    tmpSWL.push_back(*(new StringWithLimits(SAT.who,tmpPosition,this->scale*0.5f,this->maximumWidth/4*0.9f,this->maximumHeight,StringWithLimits::TOP_CORNER,this->shaderProgram)));

    this->options.push_back(tmpSWL);
    this->elements.push_back(SAT.element);

    this->myOptionSelector->setInstantPosition(this->position[0],
                                               this->position[1],
                                               this->position[0]+this->maximumWidth/4,
                                               this->position[1]-this->maximumHeight/2);
    this->selectedOption=0;
}

template<class T>
void CompleteDetailsList<T>::goUp(void){
    if(this->selectedOption>0 && !this->options.empty()){
        this->selectedOption--;
        this->myOptionSelector->setInstantPosition(this->position[0],
                                                   this->options.at(this->selectedOption).front().getYPos()+this->options.at(this->selectedOption).front().getHeight()*0.2f,
                                                   this->position[0]+this->maximumWidth,
                                                   this->options.at(this->selectedOption).back().getYPos()-this->options.at(this->selectedOption).back().getHeight()-this->options.at(this->selectedOption).back().getScale()*0.4f);
    }
}

template<class T>
void CompleteDetailsList<T>::goDown(void){
    if(this->selectedOption<(this->options.size()-1) && !this->options.empty()){
        this->selectedOption++;
        this->myOptionSelector->setInstantPosition(this->position[0],
                                                   this->options.at(this->selectedOption).front().getYPos()+this->options.back().back().getHeight()*0.2f,
                                                   this->position[0]+this->maximumWidth,
                                                   this->options.at(this->selectedOption).back().getYPos()-this->options.at(this->selectedOption).back().getHeight()-this->options.at(this->selectedOption).back().getScale()*0.4f);
    }
}

template<class T>
int CompleteDetailsList<T>::getID(void){
    return this->selectedOption;
}

template<class T>
T CompleteDetailsList<T>::getSelectedElement(void){
    return this->elements.at(this->selectedOption);
}

template<class T>
void CompleteDetailsList<T>::flush(void){
    this->options.clear();
    this->elements.clear();
}

template<class T>
int CompleteDetailsList<T>::checkIfClicked(float xpos, float ypos){
    if(xpos> this->position[0] && xpos < this->maximumWidth+this->position[0]){
        if(ypos<this->position[1] && ypos > this->position[1]-this->maximumHeight){
            int selected=(int)((((xpos+this->maximumWidth/2)*4))/this->maximumWidth)+4* (int)((((this->maximumHeight/2-ypos)*2))/this->maximumHeight);
            if(selected < this->options.size()){
                return selected;
            }
        }
    }
    return -1;
}

template<class T>
int CompleteDetailsList<T>::killClicked(int whoToKill){
    if(whoToKill>=this->options.size()) return -1;

    bool killed=true;
    if(this->currentOrders.at(whoToKill).getState()==StoringArray::UNATTENDED){
        this->currentOrders.at(whoToKill).advanceState();
        killed=false;
    } else if(this->currentOrders.at(whoToKill).getState()==StoringArray::ATTENDED){
        this->currentOrders.at(whoToKill).advanceState();
        this->currentOrders.erase(this->currentOrders.begin()+whoToKill);
    }
    this->options.clear();
    this->elements.clear();
    this->myTiles.clear();
    while(!this->nextOrders.empty()){
        if(this->currentOrders.size()>7){
            break;
        } else {
            this->currentOrders.push_back(this->nextOrders.front());
            this->nextOrders.pop();
        }
    }
    for(int i=0;i<this->currentOrders.size();i++){
        loadTile(this->currentOrders.at(i));
    }
    return killed;
}

template<class T>
void CompleteDetailsList<T>::draw(float time){
    if(!this->options.empty()){
        //this->myOptionSelector->draw(time);
        for(int i = 0;i<this->myTiles.size();i++){
            this->myTiles.at(i).draw(time);
        }
        for(int i=0;i<this->options.size() && i<8;i++){
            for(int j=0;j<this->options.at(i).size();j++){
                this->options.at(i).at(j).draw(1);
            }
        }
    }
}

#endif // INCLUDE_TXTRESOURCES_H
