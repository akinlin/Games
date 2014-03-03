//
//  LevelEditorScene.h
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/15/13.
//
//

#ifndef __ProjectDrumroll__LevelEditorScene__
#define __ProjectDrumroll__LevelEditorScene__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Grid.h"
#include "HUD.h"
#include "Goals.h"
#include "InteractionMenu.h"
#include "GamePiece.h"
#include "GameScene.h"
USING_NS_CC;

class LevelEditorScene : public CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCallback(CCObject* pSender);

	// callback to set interaction selected state
	void interactionSelected(ccColor3B color, gamePieceInteractionType interactionState);
    
    // implement the "static node()" method manually
    CREATE_FUNC(LevelEditorScene);
    
private:
// Member Components
    // Menu to handle the game
    CCMenu* m_pItemMenu;
    // Grid where all the gameplay happens
    Grid* m_gridReference;
    // Background (Should probably be its own class)
    CCSprite* m_backgroundReference;
	// InteractionMenu handles user selected interactions
	GameInteractionMenu* m_interactionReference;
    
// Member Management
    // Touch state
    void refreshTouchState();
    // void update all
    void updateAll(float dx);
    
// Member helper functions
    // create the gird
    void createGrid();
};

#endif /* defined(__ProjectDrumroll__GameScene__) */
