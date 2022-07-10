#pragma once

#include "Ogre.h"
#include "OgreFont.h"
#include "OgreFontManager.h"

using namespace Ogre;

class MovableTextOverlayAttributes
{
public:
	MovableTextOverlayAttributes(const Ogre::UTFString & name, const Ogre::Camera *cam,
		const Ogre::UTFString & fontName = "Tahoma", const int &charHeight = 16, const Ogre::ColourValue & color = Ogre::ColourValue::White, const Ogre::UTFString & materialName = "");
	~MovableTextOverlayAttributes();

	void setFontName(const Ogre::UTFString & fontName);
	void setMaterialName(const Ogre::UTFString & materialName);
	void setColor(const Ogre::ColourValue & color);
	void setCharacterHeight(const unsigned int &height);

	const Ogre::UTFString& getName() const { return mName; }
	const Ogre::Camera* getCamera() const { return mpCam; }
	const Ogre::Font* getFont() const { return mpFont; }
	const Ogre::UTFString& getFontName() const { return mFontName; }
	const Ogre::UTFString& getMaterialName() const { return mMaterialName; }
	const Ogre::ColourValue& getColor() const { return mColor; }
	const Ogre::Real getCharacterHeight() const { return mCharHeight; }

	const Ogre::UTFString mName;
	const Ogre::Camera *mpCam;

	Ogre::Font* mpFont;
	Ogre::UTFString mFontName;
	Ogre::UTFString mMaterialName;
	Ogre::ColourValue mColor;
	Ogre::Real mCharHeight;
};

class MovableTextOverlay {
public:
	MovableTextOverlay(const Ogre::UTFString & name, const Ogre::UTFString & caption,
		const Ogre::MovableObject *mov, MovableTextOverlayAttributes *attrs, const Real &tLeft = 0.0, const Real &tTop = 0.0, const Real &yOffset = 0);

	virtual ~MovableTextOverlay();

	void setCaption(const Ogre::UTFString & caption);
	void setUpdateFrequency(Ogre::Real updateFrequency) { mUpdateFrequency = updateFrequency; }
	void setAttributes(MovableTextOverlayAttributes *attrs)
	{
		mAttrs = attrs;
		_updateOverlayAttrs();
	}

	const Ogre::UTFString&	getName() const { return mName; }
	const Ogre::UTFString&	getCaption() const { return mCaption; }
	const Ogre::Real getUpdateFrequency() const { return mUpdateFrequency; }
	const bool isOnScreen() const { return mOnScreen; }
	const bool isEnabled() const { return mEnabled; }
	const MovableTextOverlayAttributes* getAttributes() const { return mAttrs; }

	void enable(bool enable);
	void update(const Real &timeSincelastFrame, bool placeAtTop, const Real &borderSizeOffset = 0, bool chaseUpdate = false);

	// Needed for RectLayoutManager.
	int getPixelsTop() { return Ogre::OverlayManager::getSingleton().getViewportHeight() * (mpOvContainer->getTop()); }
	int getPixelsBottom() { return Ogre::OverlayManager::getSingleton().getViewportHeight() * (mpOvContainer->getTop() + mpOvContainer->getHeight()); }
	int getPixelsLeft() { return Ogre::OverlayManager::getSingleton().getViewportWidth() * mpOvContainer->getLeft(); }
	int getPixelsRight() { return Ogre::OverlayManager::getSingleton().getViewportWidth() * (mpOvContainer->getLeft() + mpOvContainer->getWidth()); }

	void setPixelsTop(int px) { mpOvContainer->setTop((Ogre::Real)px / Ogre::OverlayManager::getSingleton().getViewportHeight()); }
	void setPixelsLeft(int px) { mpOvContainer->setLeft((Ogre::Real)px / Ogre::OverlayManager::getSingleton().getViewportWidth()); }
	// end

protected:
	void _computeTextSize();
	void _updateOverlayAttrs();
	void _getMinMaxEdgesOfAABBIn2D(Ogre::Real& MinX, Ogre::Real& MinY, Ogre::Real& MaxX, Ogre::Real& MaxY, bool getTop);
	void _getScreenCoordinates(const Ogre::Vector3& position, Ogre::Real& x, Ogre::Real& y);

	const Ogre::UTFString mName;
	const Ogre::MovableObject* mpMov;

	Ogre::Overlay* mpOv;
	Ogre::OverlayContainer* mpOvContainer;
	Ogre::OverlayElement* mpOvText;

	// true if mpOvContainer is visible, false otherwise
	bool mEnabled;

	// true if mTextWidth needs to be recalculated
	bool mNeedUpdate;

	// Text width in pixels
	Ogre::Real mTextWidth;
	unsigned short mNoOfLines;

	// the Text
	Ogre::UTFString mCaption;

	// true if the upper vertices projections of the MovableObject are on screen
	bool mOnScreen;

	// the update frequency in seconds
	// mpOvContainer coordinates get updated each mUpdateFrequency seconds.
	Ogre::Real mUpdateFrequency;

	// the Font/Material/Color text attributes
	MovableTextOverlayAttributes *mAttrs;
	Real mYOffset;
};

