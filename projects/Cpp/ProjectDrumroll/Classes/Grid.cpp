//
//  Grid.cpp
//  ProjectDrumroll
//
//  Created by Alexander Kinlin on 8/17/13.
//
//

#include "Grid.h"
#include "GamePiece.h"
#include "GameScene.h"
#include "ScreenHelper.h"
#include "FileOperation.h"

const int GRID_SPACE = 10;
const int SINGLE_PIECE_ELIMINATION_DEDUCTION = 150;
const int COMBO_ELIMINATION_ADDITION = 50;

CCPoint blueLocation = ccp(-575, 650);
CCPoint greenLocation = ccp(-575, 400);
CCPoint yellowLocation = ccp(-575, 150);
CCPoint purpleLocation = ccp(-575, -100);

#define CCCA(x)   (x->copy()->autorelease())

Grid::Grid()
{
    // reset score
    m_highComboCount  = 0;
    m_interactionCount = 0;
	m_slideMoves = 0;
    
    // turn on touch events
    setTouchEnabled( true );

	// load the level
	int sharedlevel = CCUserDefault::sharedUserDefault()->getIntegerForKey("current_level");
	std::string levelFile;
    std::string filepath = FileOperation::getFilePath();
	if (sharedlevel == 1)
	{
		levelFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("levelA.plist");//FileOperation::getFilePath() + "levelA.plist";
	}
	else if (sharedlevel == 2)
	{
		levelFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("levelB.plist");//FileOperation::getFilePath() + "levelB.plist";
	}
	CCArray* level = NULL;
    if (CCFileUtils::sharedFileUtils()->isFileExist(levelFile))
    {
        level = CCArray::createWithContentsOfFile(levelFile.c_str());
    }
   
    // create game pieces and fill the grid
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
			// create a piece and assign it to the grid location
			GamePiece* gamePieceSprite;
			// if the level was loaded correctly
			if (level != NULL)
			{
				int color = 0;
				int type = 0;
                CCDictionary* piece = CCDictionary::createWithDictionary((CCDictionary *)level->objectAtIndex((i*GRID_COLS) + j));
                color = piece->valueForKey("COLOR")->intValue();
                type = piece->valueForKey("TYPE")->intValue();
				CCLog("ROW %d COL %d = PIECE %d TYPE %d", i, j, color, type);

				gamePieceSprite = new GamePiece(color, type);
			}
			else
			{
				// create a piece and assign it to the grid location
				gamePieceSprite = new GamePiece();
			}
            
            // set the piece location with space in between
            // set piece width and height (assuming all are the same size)
            m_pieceTextureWidth = gamePieceSprite->getTextureWidth();
            m_pieceTextureHeight = gamePieceSprite->getTextureHeight();

            int piecePosX = (j * m_pieceTextureWidth) + (VisibleRect::getScaledFont(GRID_SPACE) * j);
            int piecePoxY = (i * m_pieceTextureHeight) + (VisibleRect::getScaledFont(GRID_SPACE) * i);
            
            gamePieceSprite->setPosition(ccp(piecePosX, piecePoxY));
            gamePieceSprite->setAnchorPoint(CCPointZero);
            gridTable[i][j] = gamePieceSprite;
            
            addChild(gridTable[i][j]);
        }
    }
    // set the anchor point to zero
    setAnchorPoint(CCPointZero);
    
    // set the width and height
    // just using the size of the first piece to calculate dimensions
    m_gridWidth = (m_pieceTextureWidth * GRID_COLS) + (VisibleRect::getScaledFont(GRID_SPACE) * GRID_COLS);
    m_gridHeight = (m_pieceTextureHeight * GRID_ROWS) + (VisibleRect::getScaledFont(GRID_SPACE) * GRID_ROWS);
    
    // set content size to grid size, assuming all pieces are the same size
    setContentSize(CCSizeMake(m_gridWidth, m_gridHeight));
    
	// init GamePiece reference
    m_interactionGamePiece = NULL;
}

Grid::~Grid()
{
    // destroy all of the game pieces in the grid
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            // if there is a piece at this location destroy it
            if (gridTable[i][j] != NULL)
            {
                delete gridTable[i][j];
            }
        }
    }
}

void Grid::startParticleAnim(CCPoint location, int color)
{
	// get the end point
	CCPoint endLocation;
	if (color == pieceColorPurple)
	{
		endLocation = blueLocation;
	}
	else if (color == pieceColorGreen)
	{
		endLocation = greenLocation;
	}
	else if (color == pieceColorYellow)
	{
		endLocation = yellowLocation;
	}
	else
	{
		// pieceColorRed
		endLocation = purpleLocation;
	}

	CCParticleSystem* emitter = CCParticleMeteor::create();
	addChild(emitter, 100);

	emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	emitter->setPosition(location);

	// delta point
	CCPoint dxPoint = ccp(endLocation.x - location.x, endLocation.y - location.y);
	CCActionInterval* move_ease_in = CCEaseIn::create(CCMoveTo::create(1, endLocation), 2.5f);
	int *colour = new int(color);
	emitter->runAction(CCSequence::create(move_ease_in, CCCallFuncND::create(this, callfuncND_selector(Grid::callback1), (void*)colour), NULL));
}

void Grid::callback1(CCNode* pTarget, void* data)
{
	// get the emitter
	CCParticleSystem* emitter = (CCParticleSystem *)(pTarget);
	emitter->stopSystem();

	CCPoint placement = emitter->getPosition();

	emitter = CCParticleExplosion::create();
	addChild(emitter, 100);

	emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	emitter->setAutoRemoveOnFinish(true);
	emitter->setPosition(placement);

	// add to the bar level
	GameScene* parent = (GameScene*)this->getParent();
	if (parent != NULL)
	{
		int *color = (int*)data;
		parent->addToBar(*color, 1);
		delete color;
	}
}

GamePiece* Grid::getGamePieceAtIndex(int row, int col)
{
    if (row < GRID_ROWS && row >= 0 && col < GRID_COLS && col >= 0)
    {
        return gridTable[row][col];
    }
    return NULL;
}

CCPoint Grid::getIndexAtGamePiece(GamePiece* basePiece)
{
    // get the piece global location
    CCPoint globalPostion = basePiece->getPosition();
    CCLog("Global Location row:%f col:%f", globalPostion.y, globalPostion.x);
    
    // get the referenced piece width and height
    int pieceWidth = getPieceWidth();
    int pieceHeight = getPieceHeight();
    
    // get the row and col index of the piece
    int row = globalPostion.y/pieceWidth;
    int col = globalPostion.x/pieceHeight;
    CCPoint indexPoint = ccp(col,row);
    CCLog("Created Point row:%f col:%f", indexPoint.y, indexPoint.x);
    
    return indexPoint;
}

GamePiece* Grid::getGamePieceAtLocation(CCPoint p)
{
    // check if location is within grid bounds
    if (getPositionX() <= p.x && getPositionX() + getWidth() >= p.x)
    {
        if (getPositionY() <= p.y && getPositionY() + getHeight() >= p.y)
        {
            // touch location is within the grid
            // get the referenced piece width and height
            int pieceWidth = getPieceWidth();
            int pieceHeight = getPieceHeight();
            
            // get the row and col index of the piece
            int row = (p.y-getPositionY())/pieceWidth;
            int col = (p.x-getPositionX())/pieceHeight;
            CCLog("ROWCOL: x=%d y=%d", row, col);
            
            // get the piece at that location
            return getGamePieceAtIndex(row, col);
        }
    }
    return NULL;
}

float Grid::getWidth()
{
    // assuming all pieces are the same size
    return m_gridWidth;
}

float Grid::getHeight()
{
    // assuming all pieces are the same size
    return m_gridHeight;
}

void Grid::handleTouch(CCPoint p)
{
    // find the piece selected
    GamePiece* gamePieceSprite = getGamePieceAtLocation(p);
	TouchSelectorStateMachine* sharedTouchSelector = TouchSelectorStateMachine::sharedTouchSelector();
    if (gamePieceSprite != NULL)
    {
        if (gamePieceSprite->isVisible())
        {
            // handle the touch according to touchState
			if (sharedTouchSelector->getTouchState() == interact)
            {
                // handle the interaction type
				switch (sharedTouchSelector->getInteractionState())
                {
                    // pieceInteractionSwitch
                    case pieceInteractionSwitch:
						if (m_interactionGamePiece != NULL && sharedTouchSelector->activeMultiTouchInteraction())
                        {
                            handleSwitchInteraction(gamePieceSprite);
							sharedTouchSelector->setInteractionState(pieceInteractionEmpty);
                            m_interactionGamePiece = NULL;
							sharedTouchSelector->setActiveMultiTouchInteraction(false);
                        }
                        else
                        {
                            m_interactionGamePiece = gamePieceSprite;
							sharedTouchSelector->setActiveMultiTouchInteraction(true);
                        }
                        break;
                        
                    case pieceInteractionFlip:
                        // flip the piece
                        handleFlipInteraction(gamePieceSprite);
                        break;
                        
                    case pieceInteractionDPadFlip:
                        handleDPadFlipInteraction(gamePieceSprite);
                        break;
                      
                    default:
                        break;
                }
                
            }
            else
            {
                // do something with it to test the touch location
                int comboCount = eliminateGamePieces(gamePieceSprite,0);
                if (comboCount == 0)
                {
                    // only one piece was eliminated
					subPoints(SINGLE_PIECE_ELIMINATION_DEDUCTION);
                }
                else
                {
                    // a combo of two or more was created
					addPoints((COMBO_ELIMINATION_ADDITION*(comboCount + 1)));

					startParticleAnim(gamePieceSprite->getPosition(), gamePieceSprite->getPieceColor());

					// fill the interaction bar with the comboCount/2 * 2
					/*int barAddition = comboCount;
					int pieceColor = gamePieceSprite->getPieceColor();
					GameScene* parent = (GameScene*)this->getParent();
					parent->addToBar(pieceColor, barAddition);*/
                }
                
                // set the high combo
                if (comboCount > m_highComboCount)
                {
                    m_highComboCount = comboCount;
                }

				// reset the pieces in the grid
				recalculateGrid();

				sharedTouchSelector->setInteractionState(pieceInteractionEmpty);
            }
        }
		// action occured points/values need to be adjusted
		sharedTouchSelector->setActionReset(false);
    }
	else
	{
		// piece is NULL assuming this is because the location was outside of the grid
		// this assumption is based on the fact the getGamePieceAtLocation returns NULL in that case
		// reset the touch state
		sharedTouchSelector->setInteractionState(pieceInteractionEmpty);
		// reset the multitouch state
		sharedTouchSelector->setActiveMultiTouchInteraction(false);
		// no action occured dont reduce points/values
		sharedTouchSelector->setActionReset(true);
	}
}

// TODO: This function is bloated as hell. The logic is expanded for readability and debugging.
// Once this is verified as solid it should be moved into a less readable but more efficient form.
int Grid::eliminateGamePieces(GamePiece* basePiece, int comboCount)
{
    CCLog("^^^^^^^^^^^");
    // check all linked pieces and remove them
    // run a recursive function checking for matching pieces in the following order:
    // up, right, down, left (clockwise)
    if(basePiece != NULL)
    {
        // set the flag to indicate the piece is in an eliminate loop check
        basePiece->setElinationCheck(true);
        
        // get the grid location of the basepiece
        CCPoint basePieceindex = getIndexAtGamePiece(basePiece);
        
        // check up
        float aboveRow = basePieceindex.y + 1.0f;
        CCLog("Check piece row:%f col:%f", basePieceindex.y, basePieceindex.x);
        CCLog("Above piece row:%f col:%f", aboveRow, basePieceindex.x);
        
        // really only need to check if the index is out of bounds in one direction since the
        // basePieceindex is assumed to be valid, but checking both for completeness.
        // similar checks for the rest of the tests
        if (aboveRow > 0 && aboveRow < GRID_ROWS)
        {
            GamePiece* abovePiece = getGamePieceAtIndex(aboveRow, basePieceindex.x);
            // first make sure the piece hasn't already been checked
            if (abovePiece != NULL)
            {
                CCPoint abovePieceLocation = abovePiece->getPosition();
                CCLog("Actual above piece row:%f col:%f", abovePieceLocation.y, abovePieceLocation.x);
                if (!abovePiece->isInElinationCheck() && abovePiece->isActive())
                {
                    if (basePiece->getPieceColor() == abovePiece->getPieceColor())
                    {
                        CCLog("UFOUND MATCH");
                        // piece above matches trigger recursive check with this as a the base piece
						int returnedCombo = eliminateGamePieces(abovePiece, comboCount + 1);
						comboCount = MAX(comboCount, returnedCombo);
						startParticleAnim(abovePieceLocation, abovePiece->getPieceColor());
                    }
                    else
                    {
                        CCLog("UNO MATCH FOUND");
                    }
                }
                else
                {
                    CCLog("UPIECE ALREADY CHECKED");
                }
            }
            else
            {
                CCLog("UPIECE NOT VALID");
            }
        }
        else
        {
            CCLog("UPIECE OUT OF BOUNDS");
        }
        
        // check right
        float rightCol = basePieceindex.x + 1.0f;
        CCLog("Check piece row:%f col:%f", basePieceindex.y, basePieceindex.x);
        CCLog("Piece to the right row:%f col:%f", basePieceindex.y, rightCol);
        
        // really only need to check if the index is out of bounds in one direction since the
        // basePieceindex is assumed to be valid, but checking both for completeness.
        // similar checks for the rest of the tests
        if (rightCol > 0 && rightCol < GRID_COLS)
        {
            GamePiece* rightPiece = getGamePieceAtIndex(basePieceindex.y, rightCol);
            CCPoint rightPieceLocation = rightPiece->getPosition();
            CCLog("Actual right piece row:%f col:%f", rightPieceLocation.y, rightPieceLocation.x);
            // first make sure the piece hasn't already been checked
            if (rightPiece != NULL)
            {
				if (!rightPiece->isInElinationCheck() && rightPiece->isActive())
                {
                    if (basePiece->getPieceColor() == rightPiece->getPieceColor())
                    {
                        CCLog("RFOUND MATCH");
                        // piece above matches trigger recursive check with this as a the base piece
						int returnedCombo = eliminateGamePieces(rightPiece, comboCount + 1);
						comboCount = MAX(comboCount, returnedCombo);
						startParticleAnim(rightPieceLocation, rightPiece->getPieceColor());
                    }
                    else
                    {
                        CCLog("RNO MATCH FOUND");
                    }
                }
                else
                {
                    CCLog("RPIECE ALREADY CHECKED");
                }
            }
            else
            {
                CCLog("RPIECE NOT VALID");
            }
        }
        else
        {
            CCLog("RPIECE OUT OF BOUNDS");
        }
        
        // check down
        float downRow = basePieceindex.y - 1.0f;
        //CCLog("Check piece row:%f col:%f", basePieceindex.y, basePieceindex.x);
        //CCLog("Down piece row:%f col:%f", downRow, basePieceindex.x);
        
        // really only need to check if the index is out of bounds in one direction since the
        // basePieceindex is assumed to be valid, but checking both for completeness.
        // similar checks for the rest of the tests
        if (downRow >= 0 && downRow < GRID_ROWS)
        {
            GamePiece* downPiece = getGamePieceAtIndex(downRow, basePieceindex.x);
            CCPoint downPieceLocation = downPiece->getPosition();
            //CCLog("Actual down piece row:%f col:%f", downPieceLocation.y, downPieceLocation.x);
            // first make sure the piece hasn't already been checked
            if (downPiece != NULL)
            {
				if (!downPiece->isInElinationCheck() && downPiece->isActive())
                {
                    if (basePiece->getPieceColor() == downPiece->getPieceColor())
                    {
                        //CCLog("FOUND MATCH");
                        // piece above matches trigger recursive check with this as a the base piece
						int returnedCombo = eliminateGamePieces(downPiece, comboCount + 1);
						comboCount = MAX(comboCount, returnedCombo);
						startParticleAnim(downPieceLocation, downPiece->getPieceColor());
                    }
                    else
                    {
                        //CCLog("NO MATCH FOUND");
                    }
                }
                else
                {
                    //CCLog("PIECE ALREADY CHECKED");
                }
            }
            else
            {
                //CCLog("PIECE NOT VALID");
            }
        }
        else
        {
            //CCLog("PIECE OUT OF BOUNDS");
        }
        
        // check left
        float leftCol = basePieceindex.x - 1.0f;
        //CCLog("Check piece row:%f col:%f", basePieceindex.y, basePieceindex.x);
        //CCLog("Piece to the left row:%f col:%f", basePieceindex.y, leftCol);
        
        // really only need to check if the index is out of bounds in one direction since the
        // basePieceindex is assumed to be valid, but checking both for completeness.
        // similar checks for the rest of the tests
        if (leftCol >= 0 && leftCol < GRID_COLS)
        {
            GamePiece* leftPiece = getGamePieceAtIndex(basePieceindex.y, leftCol);
            CCPoint leftPieceLocation = leftPiece->getPosition();
            //CCLog("Actual right piece row:%f col:%f", leftPieceLocation.y, leftPieceLocation.x);
            // first make sure the piece hasn't already been checked
            if (leftPiece != NULL)
            {
				if (!leftPiece->isInElinationCheck() && leftPiece->isActive())
                {
                    if (basePiece->getPieceColor() == leftPiece->getPieceColor())
                    {
                        //CCLog("FOUND MATCH");
                        // piece above matches trigger recursive check with this as a the base piece
						int returnedCombo = eliminateGamePieces(leftPiece, comboCount + 1);
						comboCount = MAX(comboCount, returnedCombo);
						startParticleAnim(leftPieceLocation, leftPiece->getPieceColor());
                    }
                    else
                    {
                        //CCLog("NO MATCH FOUND");
                    }
                }
                else
                {
                    //CCLog("PIECE ALREADY CHECKED");
                }
            }
            else
            {
                //CCLog("PIECE NOT VALID");
            }
        }
        else
        {
            //CCLog("PIECE OUT OF BOUNDS");
        }

        // reset the flag to indicate the piece is in an eliminate loop check
        basePiece->setElinationCheck(false);
        
        // remove the piece
        // try just deleting it (hopefully the rest of the code has checks for valid pieces)
        //delete basePiece;
        basePiece->setVisible(false);
        basePiece->setActive(false);
        
        //CCLog("COMBO COUNT: %d", comboCount);
    }
    else
    {
        //CCLog("BASE NOT VALID");
    }
    //CCLog("XX^^^^^^^^^^^^^XX");
    
    return comboCount;
}

void Grid::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
    // Only handle the touch start if the touch state is interaction
    // Skip setting the state if interactionState is pieceInteractionSwitch
	TouchSelectorStateMachine* sharedTouchSelector = TouchSelectorStateMachine::sharedTouchSelector();
	if (sharedTouchSelector->getTouchState() == interact && sharedTouchSelector->getInteractionState() != pieceInteractionSwitch)
    {
        // Set the current grid state depending on the interaction type
        CCSetIterator it;
        CCTouch* touch;
        
        for( it = touches->begin(); it != touches->end(); it++)
        {
            touch = (CCTouch*)(*it);
            
            if(!touch)
                break;
            
            CCPoint location = touch->getLocationInView();
            location = CCDirector::sharedDirector()->convertToGL(location);

			// reset the slideMoveDelta
			m_slideLastPoint = 0;
            
            // get the game piece that was selected
            GamePiece *selectedGamePiece = getGamePieceAtLocation(location);
			m_interactionGamePiece = selectedGamePiece;
            if (selectedGamePiece)
            {
                // HARD CODED FIX IT SOON
                switch (selectedGamePiece->getInteractionType())
                {
                    case 1:
						sharedTouchSelector->setInteractionState(pieceInteractionFlip);
                        break;
                        
                    case 2:
						sharedTouchSelector->setInteractionState(pieceInteractionDPadFlip);
                        break;
                        
                    case 3:
						sharedTouchSelector->setInteractionState(pieceInteractionSwitch);
                        break;

					case 4:
						sharedTouchSelector->setInteractionState(pieceInteractionSlide);
						break;

					case 5:
						sharedTouchSelector->setInteractionState(pieceInteractionRotary);
						break;
                        
                    default:
						// keep whatever was set previously
                        break;
                }
                // piece found break the loop
                break;
            }
        }
    }

	CCLog("Grid ccTouchesBegan");
}

void Grid::ccTouchesMoved(CCSet *touches, CCEvent *event)
{
    // Check the current grid state and handle the move event
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        // FIX IT SOON
		switch (TouchSelectorStateMachine::sharedTouchSelector()->getInteractionState())
        {
                // pieceInteractionSlide
            case pieceInteractionSlide:
                handleSlideMove(location);
                break;
                
                // pieceInteractionRotary
            case pieceInteractionRotary:
                //handleRotaryMove(location);
                break;
                
                // pieceInteractionFlip and pieceInteractionSwitch default to nothing on move
            default:
                break;
        }
    }

	CCLog("Grid ccTouchesMoved");
}

void Grid::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // Check the current grid state and handle the end event
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        // legacy function handleTouch doesn't account for grid state
        handleTouch( location );

		if (TouchSelectorStateMachine::sharedTouchSelector()->getInteractionState() == pieceInteractionSlide)
		{
			handleSlideComplete();

			// clear the interaction piece
			m_interactionGamePiece = NULL;
		}
    }

	CCLog("Grid ccTouchesEnded");
}

void Grid::ccTouchesCancelled(CCSet *touches, CCEvent *event)
{
	CCLog("Grid ccTouchesCancelled");
}

bool Grid::isLevelComplete()
{
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            // check each gamePiece in the grid and if all are not visible then return true
            // otherwise return false
            GamePiece* gamePieceSprite = gridTable[i][j];
            if (gamePieceSprite->isVisible())
            {
                return false;
            }
        }
    }
    return true;
}

int Grid::getInteractionCount()
{
    return m_interactionCount;
}

int Grid::getComboCount()
{
    return m_highComboCount;
}

void Grid::setPieceLocaiton(int row, int col)
{
    
}

// returns the width of the piece referenced with grid padding
// assumes all pieces are the same size
int Grid::getPieceWidth()
{
    return m_pieceTextureWidth + VisibleRect::getScaledFont(GRID_SPACE);
}

// returns the height of the piece referenced with grid padding
// assumes all pieces are the same size
int Grid::getPieceHeight()
{
    return m_pieceTextureHeight + VisibleRect::getScaledFont(GRID_SPACE);
}

// remove the pieces that are not active and drop the pieces above
void Grid::recalculateGrid()
{
    // run through each column and move all of the pieces down if non-active pieces are below
    CCPoint firstNonActive;
    CCPoint firstNonActivePosition = CCPointZero;
    int numPreviousEmptyCols = 0;
    for (int i = 0; i < GRID_COLS; i++)
    {
        // check if the previous row was empty
        if (firstNonActive.x == -1)
        {
            // increment the previous row empty count
            numPreviousEmptyCols++;
        }
        else
        {
            // if previous rows are not empty then reset the pevious row empty counter
            numPreviousEmptyCols = 0;
        }
        
        // reset the point
        firstNonActive = ccp(-1,-1);
        for (int j = 0; j < GRID_ROWS; j++)
        {
            // check if the piece is not active
            if (!gridTable[j][i]->isActive())
            {
                // found a non active piece set the firstNoActive Point if not already set
                if (firstNonActive.x == -1)
                {
                    firstNonActive = ccp(j,i);
                    firstNonActivePosition = gridTable[j][i]->getPosition();
                }
            }
            else
            {
                // piece is active check if point is set
                if (firstNonActive.x != -1)
                {
                    // switch the piece to the new table reference
                    int row = firstNonActive.x;
                    int col = firstNonActive.y;
                    // set first non active piece to the location of the new active piece
                    gridTable[row][col]->setPosition(gridTable[j][i]->getPosition());
                              
                    // move the active piece to the firstNonActive location
                    gridTable[j][i]->setPosition(firstNonActivePosition);
                    
                    // switch the piece references in the grid
                    GamePiece* emptySpace = gridTable[row][col];
                    gridTable[row][col] = gridTable[j][i];
                    gridTable[j][i] = emptySpace;
                    
                    // update the firstNonActive point
                    CCPoint testPoint = gridTable[row][col]->getPosition();
                    row++;
                    if (row < GRID_ROWS)
                    {
                        firstNonActive = ccp(row, col);
                        firstNonActivePosition = ccp(firstNonActivePosition.x, getPieceHeight()*row);
                    }
                }
            }
        }
    }
    
    // find all empty rows and move pieces to the left
    int emptyColumnCount = 0;
    for (int i = 0; i < GRID_COLS; i++)
    {
        // check the first game piece is active
        if (!gridTable[0][i]->isActive())
        {
            // assume this entire row is empty
            emptyColumnCount++;
        }
        else
        {
            // the game piece is active
            // check to for a column movement
            if (emptyColumnCount > 0)
            {
				for (int k = 0; k < emptyColumnCount; k++)
				{
					// move the active column to the left
					// loop all of the pieces in the column until an empty piece is encountered
					for (int j = 0; j < GRID_ROWS; j++)
					{
						// check for the first inactive piece
						if (!gridTable[j][i]->isActive())
						{
							// if the piece is inactive break the loop
							break;
						}

						//                    // need a temp holder piece
						//                    GamePiece gamePieceHolder = *gridTable[j][i - emptyColumnCount];
						//                    // replace the empty piece pointer with the active piece
						//                    gridTable[j][i - emptyColumnCount] = gridTable[j][i];
						//                    // replace the moved active piece with the empty piece stored in holder
						//                    gridTable[j][i] = &gamePieceHolder;

						// fix the positions
						CCPoint positionHolder = gridTable[j][i - emptyColumnCount]->getPosition();
						gridTable[j][i - emptyColumnCount]->setPosition(gridTable[j][i]->getPosition());
						gridTable[j][i]->setPosition(positionHolder);

						// switch the pointers
						GamePiece* pointerHolder = gridTable[j][i - emptyColumnCount];
						gridTable[j][i - emptyColumnCount] = gridTable[j][i];
						gridTable[j][i] = pointerHolder;

						//gridTable[j][i]->setColor(ccBLUE);
					}
					// hacky decrement the i counter
					i--;
				}
            }
			// reset the counter
			emptyColumnCount = 0;
        }
    }
}

void Grid::addPoints(int points)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_score") + points);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_add_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_add_score") + points);
}

void Grid::subPoints(int points)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_score") - points);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("current_sub_score", CCUserDefault::sharedUserDefault()->getIntegerForKey("current_sub_score") + points);
}

void Grid::handleSlideMove(CCPoint location)
{
    // slide the row or column until the user releases the touch

	// Make sure the touch is still within the grid bounds
	if (location.x >= getPositionX() - 1 && location.x <= getPositionX() + m_gridWidth + 1)
	{
		// get the delta of the piece movement
		int locationInGridSpace = (location.x - getPositionX());
		int deltaX = locationInGridSpace - m_slideLastPoint;
		if (m_slideLastPoint == 0)
		{
			deltaX = 0;
		}
		m_slideLastPoint = locationInGridSpace;

		// get the row the interaction piece is in
		CCPoint gridToMove = getIndexAtGamePiece(m_interactionGamePiece);
		int rowToMove = gridToMove.y;
		GamePiece* currPiece = NULL;
		// get the pieces in that row
		for (int col = 0; col < GRID_COLS; col++)
		{
			currPiece = getGamePieceAtIndex(rowToMove, col);

			// get the current location of the selected piece
			CCPoint currLoc = currPiece->getPosition();

			// handle piece wrap
			int gridPosX = getPositionX();
			if (currPiece->getPositionX() + deltaX + (currPiece->getTextureWidth()/2) <= 0)
			{
				// needs to wrap to the end
				// set the location of the last piece in the row as orginally added to grid
				int xPos = ((GRID_COLS - 1) * m_pieceTextureWidth) + (VisibleRect::getScaledFont(GRID_SPACE) * (GRID_COLS - 1)) + ((currPiece->getTextureWidth() / 2) + VisibleRect::getScaledFont(GRID_SPACE));
				currPiece->setPosition(ccp(xPos, currLoc.y));

				m_slideMoves--;
			}
			else if (currPiece->getPositionX() + deltaX >= m_gridWidth - (currPiece->getTextureWidth() / 2))
			{
				// need to wrap to the front
				// set the location of the first piece in the row as orginally added to grid
				currPiece->setPosition(ccp(-(currPiece->getTextureWidth() / 2), currLoc.y));

				m_slideMoves++;
			}
			else
			{
				// move normally
				// set the location of the selected piece to the new location
				currPiece->setPosition(ccp(currPiece->getPositionX() + deltaX, currLoc.y));
			}
		}
		// offset all the pieces in the row by the delta
	}
}

void Grid::handleSlideComplete()
{
	
	// get the row the interaction piece is in
	CCPoint gridToMove = getIndexAtGamePiece(m_interactionGamePiece);
	int rowToMove = gridToMove.y;

	// loop through the row and exchange the pieces
	GamePiece* tempArray[GRID_COLS];
	for (int i = 0; i < GRID_COLS; i++)
	{
		int newcol = i - m_slideMoves;
		if (newcol < 0)
		{
			newcol = GRID_COLS + newcol;
		}
		else if (newcol >= GRID_COLS)
		{
			newcol = newcol % GRID_COLS;
		}

		tempArray[i] = gridTable[rowToMove][newcol];
	}

	bool placementDone = false;
	for (int i = 0; i < GRID_COLS; i++)
	{
		gridTable[rowToMove][i] = tempArray[i];
		
		// put the pieces in the right place
		gridTable[rowToMove][i]->setPositionX((i * m_pieceTextureWidth) + (VisibleRect::getScaledFont(GRID_SPACE) * i));
	}

	recalculateGrid();

	m_slideMoves = 0;

	// reset the interactionState
	TouchSelectorStateMachine::sharedTouchSelector()->setInteractionState(pieceInteractionEmpty);

	// increment the interaction count
	m_interactionCount++;
}

void Grid::handleRotaryMove(CCPoint location)
{
    // rotate the pieces in a rotary movement
    // this one might be tricky since it relies on the user input
    // this is probably best for a grid state to account for user input
}

void Grid::handleRotaryComplete()
{
	// reset the interactionState
	TouchSelectorStateMachine::sharedTouchSelector()->setInteractionState(pieceInteractionEmpty);

	// increment the interaction count
	m_interactionCount++;
}

void Grid::handleSwitchInteraction(GamePiece* gamePieceSprite)
{
    // switch two pieces location
    // need the gridTable pointers to switch the pieces
    CCPoint firstSelectedGamePieceIndex = getIndexAtGamePiece(m_interactionGamePiece);
    CCPoint secondSelectedGamePieceIndex = getIndexAtGamePiece(gamePieceSprite);
    GamePiece* firstGamePiece = getGamePieceAtIndex(firstSelectedGamePieceIndex.x, firstSelectedGamePieceIndex.y);
    GamePiece* secondGamePiece = getGamePieceAtIndex(secondSelectedGamePieceIndex.x, secondSelectedGamePieceIndex.y);
    int firstRow = firstSelectedGamePieceIndex.y;
    int firstCol = firstSelectedGamePieceIndex.x;
    int secondRow = secondSelectedGamePieceIndex.y;
    int secondCol = secondSelectedGamePieceIndex.x;
    
    // get the first selected gamepiece's location
    CCPoint selectedLocation = gridTable[firstRow][firstCol]->getPosition();
    // set the first selected gamepiece's location to the location of the second piece
    gridTable[firstRow][firstCol]->setPosition(gridTable[secondRow][secondCol]->getPosition());
    // set the second selected gamepiece's location to the saved location
    gridTable[secondRow][secondCol]->setPosition(selectedLocation);
    
    // switch the pointers
    GamePiece* pointerHolder = gridTable[firstRow][firstCol];
    gridTable[firstRow][firstCol] = gridTable[secondRow][secondCol];
    gridTable[secondRow][secondCol] = pointerHolder;

	// reset the interactionState
	TouchSelectorStateMachine::sharedTouchSelector()->setInteractionState(pieceInteractionEmpty);

	// increment the interaction count
	m_interactionCount++;
}

void Grid::handleFlipInteraction(GamePiece* gamePieceSprite)
{
    // single piece flip interaction
    // since this is an instant action no state is required
    gamePieceSprite->switchToNextPiece();
    
    // reset the interactionState
	TouchSelectorStateMachine::sharedTouchSelector()->setInteractionState(pieceInteractionEmpty);
    
    // increment the interaction count
    m_interactionCount++;
}

void Grid::handleDPadFlipInteraction(GamePiece* gamePieceSprite)
{
    // DPad flip interaction
    // since this is an instant action no state is required
    // do something with it to test the touch location
    
    // change the pieces to the top/bottom/left/right
    CCPoint selectedIndex = getIndexAtGamePiece(gamePieceSprite);
    CCLog("Index x:%f y:%f", selectedIndex.x, selectedIndex.y);
    // check up
    GamePiece* gamePieceHolder = getGamePieceAtIndex(selectedIndex.y + 1, selectedIndex.x);
    if(gamePieceHolder != NULL)
    {
        gamePieceHolder->switchToNextPiece();
    }
    
    // check right
    gamePieceHolder = getGamePieceAtIndex(selectedIndex.y, selectedIndex.x + 1);
    if(gamePieceHolder != NULL)
    {
        gamePieceHolder->switchToNextPiece();
    }
    
    // check bottom
    gamePieceHolder = getGamePieceAtIndex(selectedIndex.y - 1, selectedIndex.x);
    if(gamePieceHolder != NULL)
    {
        gamePieceHolder->switchToNextPiece();
    }
    
    // check left
    gamePieceHolder = getGamePieceAtIndex(selectedIndex.y, selectedIndex.x - 1);
    if(gamePieceHolder != NULL)
    {
        gamePieceHolder->switchToNextPiece();
    }
    
    // reset the interactionState
	TouchSelectorStateMachine::sharedTouchSelector()->setInteractionState(pieceInteractionEmpty);
    
    // increment the interaction count
    m_interactionCount++;
}

