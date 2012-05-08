/*

    Qt OpenGL Tutorial - Lesson 11

    nehewidget.cpp
    v 1.00
    2002/12/20

    Copyright (C) 2002 Cavendish
                       cavendish@qiliang.net
                       http://www.qiliang.net/nehe_qt

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

*/

#include <math.h>
#include <qimage.h>

#include "nehewidget.h"

NeHeWidget::NeHeWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent, name )
{
  xRot = yRot = zRot = 0.0;
  hold = 0.0;
  
  wiggle_count = 0;
  
  fullscreen = fs;
  setGeometry( 0, 0, 640, 480 );
  setCaption( "bosco & NeHe's Waving Texture Tutorial" );

  if ( fullscreen )
    showFullScreen();

  startTimer( 5 );
}

NeHeWidget::~NeHeWidget()
{
}

void NeHeWidget::initializeGL()
{
  loadGLTextures();

  glEnable( GL_TEXTURE_2D );
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.0, 0.0, 0.0, 0.5 );
  glClearDepth( 1.0 );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glPolygonMode( GL_BACK, GL_FILL );
  glPolygonMode( GL_FRONT, GL_LINE );
  
  for ( int x = 0; x < 45; x++ )
  {
    for ( int y = 0; y < 45; y++ )
    {
      points[x][y][0] = float( ( x/5.0 ) - 4.5 );
      points[x][y][1] = float( ( y/5.0 ) - 4.5 );
      points[x][y][2] = float( sin( ( ( ( x/5.0 ) * 40.0 )/360.0 ) * 3.141592654 * 2.0 ) );
    }
  }
}

void NeHeWidget::paintGL()
{
  int x, y;
  float float_x, float_y, float_xb, float_yb;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  
  glTranslatef( 0.0, 0.0, -12.0 );

  glRotatef( xRot, 1.0, 0.0, 0.0 );
  glRotatef( yRot, 0.0, 1.0, 0.0 );
  glRotatef( zRot, 0.0, 0.0, 1.0 );

  glBindTexture( GL_TEXTURE_2D, texture[0] );

  glBegin( GL_QUADS );
    for ( x = 0; x < 44; x++ )
    {
      for ( y = 0; y < 44; y++ )
      {
        float_x = float(x)/44.0;
        float_y = float(y)/44.0;
        float_xb = float(x+1)/44.0;
        float_yb = float(y+1)/44.0;
        
        glTexCoord2f( float_x, float_y );
        glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

        glTexCoord2f( float_x, float_yb );
        glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

        glTexCoord2f( float_xb, float_yb );
        glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

        glTexCoord2f( float_xb, float_y );
        glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
      }
    }
  glEnd();
  
  if ( wiggle_count == 2 )
  {
    for ( y = 0; y < 45; y++ )
    {
      hold = points[0][y][2];
      for ( x = 0; x < 44; x++ )
      {
        points[x][y][2] = points[x+1][y][2];
      }
      points[44][y][2] = hold;
    }
    wiggle_count = 0;
  }

  wiggle_count++;

  xRot += 0.3;
  yRot += 0.2;
  zRot += 0.4;
}

void NeHeWidget::resizeGL( int width, int height )
{
  if ( height == 0 )
  {
    height = 1;
  }
  glViewport( 0, 0, (GLint)width, (GLint)height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void NeHeWidget::timerEvent(QTimerEvent*)
{
  updateGL();
}

void NeHeWidget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Qt::Key_F2:
    fullscreen = !fullscreen;
    if ( fullscreen )
    {
      showFullScreen();
    }
    else
    {
      showNormal();
      setGeometry( 0, 0, 640, 480 );
    }
    update();
    break;
  case Qt::Key_Escape:
    close();
  }
}

void NeHeWidget::loadGLTextures()
{
  QImage tex, buf;
  if ( !buf.load( "./data/Tim.bmp" ) )
  {
    qWarning( "Could not read image file, using single-color instead." );
    QImage dummy( 128, 128, 32 );
    dummy.fill( Qt::green.rgb() );
    buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat( buf );
  
  glGenTextures( 1, &texture[0] );
  
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
}
