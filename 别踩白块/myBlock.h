//
//  myBlock.h
//  testgame4455
//
//  Created by stdio on 15/9/1.
//
//

#ifndef __testgame4455__myBlock__
#define __testgame4455__myBlock__

//#include <stdio.h>
#include <iostream>
#include <cocos2d.h>

USING_NS_CC;

class Block:public Sprite {
    
private:
    static Vector<Block*> * blocks;
    int lineIndex;
    
public:
    
    //方块初始化 包括颜色值 尺寸 呈现的字符 字号大小  字体颜色
    virtual bool initWithArgs(Color3B color,Size size,std::string label,float fontSize,Color4B textColor);
    //静态的创建方法
    static Block* createWithArgs(Color3B color,Size size,std::string label,float fontSize,Color4B textColor);
    
    
    static Vector<Block*> * getBlocks();
    static void removeAllBlocks();
    
    
    
    void removeBlock();
    
    int getLineIndex();
    void setLineIndex(int lineIndex);
    
    void moveDown();
};


#endif /* defined(__testgame4455__myBlock__) */
