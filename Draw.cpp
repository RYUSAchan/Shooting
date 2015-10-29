//Draw.cpp

#include "Draw.h"

DxFont::DxFont()
	:	font( NULL )
{
	Create( 16, _T( "MS UI Gothic" ), FALSE );
}

DxFont::DxFont( int fontsize )
	:	font( NULL )
{
	Create( fontsize, _T( "MS UI Gothic" ), FALSE);
}

DxFont::DxFont( int fontsize, LPCTSTR fontname, BOOL Italic )
	:	font( NULL )
{
	Create( fontsize, fontname, Italic );
}

DxFont::~DxFont()
{
	RELEASE( font );
}

BOOL DxFont::Create( int fontsize, LPCTSTR fontname, BOOL Italic )
{
	RELEASE( font );

	int result = D3DXCreateFont(
		pD3Ddevice, fontsize, NULL, FW_DONTCARE,
		NULL, Italic, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		fontname, &font );

	return ( result == 0 ) ? FALSE : TRUE;
}

void DxFont::Draw( LPCTSTR text, int count, LPRECT pRect,
	DWORD Format, D3DXCOLOR Color )
{
	if( font ) {
		pSprite->Begin( NULL );

		D3DXMATRIX mtrx;
		D3DXMatrixTranslation( &mtrx, 0.0f, 0.0f, 0.0f );
		pSprite->SetTransform( &mtrx );

		font->DrawText( pSprite, text, count, pRect, Format, Color );

		pSprite->End();
	}
}

void DxFont::Draw( LPCTSTR text, int x, int y )
{
	RECT rect = { x, y, x, y };
	Draw( text, -1, &rect, DT_NOCLIP |DT_SINGLELINE,
		0xFFFFFFFF );
}

Texture::Texture()
	:	texture( NULL )
{}

Texture::Texture( LPCTSTR filename )
	:	texture( NULL )
{
	Load( filename );
}

Texture::~Texture()
{
	RELEASE( texture );
}

void Texture::GetTextureSize( int *width, int *height )
{
	*width = texwidth;
	*height = texheight;
}

BOOL Texture::Load( LPCTSTR filename )
{
	RELEASE( texture );
	BOOL b = SUCCEEDED( D3DXCreateTextureFromFile(
		pD3Ddevice, filename, &texture ) );

	if( b ) {
		D3DSURFACE_DESC desc;

		texture->GetLevelDesc( 0, &desc );

		texwidth = ( int )desc.Width;
		texheight = ( int )desc.Height;
	}
	return b;
}

DrawTexture::DrawTexture()
{
	Reset();
}

DrawTexture::DrawTexture( Texture *source )
{
	Reset();
	SetTexture( source );
}

void DrawTexture::Reset()
{
	texture = NULL;
	cpos = CP_MIDDLECENTER;
	orig_x = orig_y = 0.0f;
	texwidth = 0;
	swidth = sheight = 0;
	SetRect( &drawrect, 0, 0, 0, 0 );
}

void DrawTexture::SetTexture( Texture *source )
{
	if( !source ) { return; }
	
	texture = source;

	int h;
	texture->GetTextureSize( &texwidth, &h );
	SetSpriteSize( texwidth, h );
}

void DrawTexture::SetSpriteSize( int width, int height )
{
	swidth = width;
	sheight = height;

	SetCenterPosition( cpos );

	SetFrame( 0 );
}

void DrawTexture::GetSpriteSize( int *width, int *height )
{
	*width = swidth;
	*height = sheight;
}

void DrawTexture::SetSpriteRect( int left, int top, int right, int bottom )
{
	swidth = right - left;
	sheight = bottom - top;

	drawrect.left = left;
	drawrect.right = right;
	drawrect.top = top;
	drawrect.bottom = bottom;

	SetCenterPosition( cpos );
}

void DrawTexture::SetCenterPosition( BYTE pos )
{
	cpos = pos;

	if( pos & CP_LEFT ) {
		orig_x = 0.0f;
	} else if( pos & CP_RIGHT ) {
		orig_x = ( float )swidth;
	} else {
		orig_x = ( float )swidth / 2.0f;
	}

	if( pos & CP_TOP )
	{
		orig_y = 0.0f;
	} else if( pos & CP_BOTTOM )
	{
			orig_y = ( float )sheight;
	} else {
		orig_y = ( float )sheight / 2.0f;
	}
}

void DrawTexture::SetFrame( int frame )
{
	int d = texwidth / swidth;
	if( d == 0 ) return;

	int xpos = frame % d;
	int ypos = frame / d;

	drawrect.left = xpos * swidth;
	drawrect.right = drawrect.left + swidth;
	drawrect.top = ypos * sheight;
	drawrect.bottom = drawrect.top + sheight;
}

const void DrawTexture::Draw( float x, float y, int alpha )
{
	Draw( x, y, 1.0f, 1.0f, alpha );
}

const void DrawTexture::Draw( float x, float y, float ex, float ey, int alpha )
{
	Draw( x, y, ex, ey, 0.0f, alpha );
}

const void DrawTexture::Draw( float x, float y, float r, int alpha )
{
	Draw( x, y, 1.0f, 1.0f, r, alpha );
}

const void DrawTexture::Draw( float x, float y, float ex, float ey, float r, int alpha )
{
	if( !texture ) {
		CautionMessage( _T( "errorz" ), _T( "テクスチャが読み込まれていません" ) );
		return;
	}

	D3DXMATRIX mtrx1, mtrx2;

	D3DXMatrixTranslation( &mtrx1, -orig_x, -orig_y, 0.0f );

	if( ex != 1.0f || ey != 1.0f ) {
		D3DXMatrixScaling( &mtrx2, ex, ey, 1.0f );
		D3DXMatrixMultiply( &mtrx1, &mtrx1, &mtrx2 );
	}

	if( r != 0.0f ) {
		D3DXMatrixRotationZ( &mtrx2, r );
		D3DXMatrixMultiply( &mtrx1, &mtrx1, &mtrx2 );
	}

	D3DXMatrixTranslation( &mtrx2, x, y, 0.0f );
	D3DXMatrixMultiply( &mtrx1, &mtrx1, &mtrx2 );

	pSprite->Begin( NULL );

	pSprite->SetTransform( &mtrx1 );
	pSprite->Draw( texture->GetTexture(),
		&drawrect, NULL, NULL, 0x00FFFFFF | ( ( BYTE )alpha << 24 ) );

	pSprite->End();
}

