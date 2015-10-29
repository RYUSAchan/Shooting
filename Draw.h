//Draw.h

#pragma once

#include "GameObject.h"

#define CP_MIDDLECENTER 0	//’†‰›
#define CP_LEFT 1			//x•ûŒü¶
#define CP_CENTER 2			//x•ûŒü’†‰›
#define CP_RIGHT 4			//x•ûŒü‰E
#define CP_TOP 8			//y•ûŒüã
#define CP_MIDDLE 16		//y•ûŒü’†‰›
#define CP_BOTTOM 32		//y•ûŒü‰º

class DxFont : public GameObject
{
private:
	LPD3DXFONT font;

public:
	DxFont();
	DxFont( int fontsize );
	DxFont( int fontsize, LPCTSTR fontname, BOOL Italic );
	~DxFont();

	BOOL Create( int fontsize, LPCTSTR fontname, BOOL Italic );

	void Draw( LPCTSTR text, int x, int y );
	void Draw( LPCTSTR text, int count,
		LPRECT pRect, DWORD Format, D3DXCOLOR Color );

	const LPD3DXFONT GetD3DXFont() { return font; }
};

class Texture : public GameObject
{
private:
	LPDIRECT3DTEXTURE9 texture;
	int texwidth, texheight;

public:
	Texture();
	Texture( LPCTSTR filename );
	~Texture();

	BOOL Load( LPCTSTR filename );

	void GetTextureSize( int *width, int *height );

	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
};

class DrawTexture : public GameObject
{
private:
	Texture *texture;

	BYTE cpos;
	float orig_x, orig_y;

	int texwidth;

	int swidth, sheight;

	RECT drawrect;

	void Reset();

public:
	DrawTexture();
	DrawTexture( Texture *source );
	~DrawTexture() {}

	void SetTexture( Texture *source );
	Texture* GetTexture() { return texture; }

	void GetSize( int *width, int *height );

	const void Draw( float x, float y, int alpha = 255);
	const void Draw( float x, float y, float r, int alpha = 255 );
	const void Draw( float x, float y, float ex, float ey, int alpha = 255 );
	const void Draw( float x, float y, float ex, float ey, float r, int alpha = 255 );

	void SetCenterPosition( BYTE pos );

	const BYTE GetCenterPosition() { return cpos; }

	void GetSpriteSize( int *width, int *height );
	void SetSpriteSize( int width, int height );

	void SetSpriteRect( int left, int top, int right, int bottom );

	void SetFrame( int frame );
};
