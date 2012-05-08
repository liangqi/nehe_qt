/*

    Qt OpenGL Tutorial - Lesson 09

    nehewidget.cpp
    v 1.00
    2002/12/19

    Copyright (C) 2002 Cavendish
                       cavendish@qiliang.net
                       http://www.qiliang.net/nehe_qt

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

*/

#include <stdio.h>
#include <qimage.h>

#include "nehewidget.h"

NeHeWidget::NeHeWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent, name )
{
  xRot = yRot = zRot = 0.0;
  zoom = -15.0;
  tilt = 90.0;
  spin = 0.0;
  loop = 0;

  twinkle = false;

  fullscreen = fs;
  setGeometry( 0, 0, 640, 480 );
  setCaption( "NeHe's Animated Blended Textures Tutorial" );

  if ( fullscreen )
    showFullScreen();

  startTimer( 5 );
}

NeHeWidget::~NeHeWidget()
{
}

void NeHeWidget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  
  for ( loop = 0; loop < num; loop++ )
  {
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, zoom );
    glRotatef( tilt, 1.0, 0.0, 0.0 );
    glRotatef( star[loop].angle, 0.0, 1.0, 0.0 );
    glTranslatef( star[loop].dist, 0.0, 0.0 );
    glRotatef( -star[loop].angle, 0.0, 1.0, 0.0 );
    glRotatef( -tilt, 1.0, 0.0, 0.0 );
    
    if ( twinkle )
    {
      glColor4ub( star[(num-loop)-1].r,
         star[(num-loop)-1].g, 
         star[(num-loop)-1].b, 255 );
      glBegin( GL_QUADS );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
      glEnd();
    }
    
    glRotatef( spin, 0.0, 0.0, 1.0 );
    glColor4ub( star[loop].r, star[loop].g, star[loop].b, 255 );
    glBegin( GL_QUADS );
      glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
      glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
      glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
    glEnd();
    
    spin += 0.01;
    star[loop].angle += float(loop)/num;
    star[loop].dist -= 0.01;
    
    if ( star[loop].dist < 0.0 )
    {
      star[loop].dist += 5.0;
      star[loop].r = rand() % 256;
      star[loop].g = rand() % 256;
      star[loop].b = rand() % 256;
    }
  }
}

void NeHeWidget::timerEvent(QTimerEvent*)
{
  updateGL();
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
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  glEnable( GL_BLEND );
  
  for ( loop = 0; loop < num; loop++ )
  {
    star[loop].angle = 0.0;
    star[loop].dist = ( float(loop)/num ) * 5.0;
    star[loop].r = rand() % 256;
    star[loop].g = rand() % 256;
    star[loop].b = rand() % 256;
  }
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

void NeHeWidget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Qt::Key_T:
    twinkle = !twinkle;
    updateGL();
    break;
  case Qt::Key_Up:
    tilt -= 0.5;
    updateGL();
    break;
  case Qt::Key_Down:
    tilt += 0.5;
    updateGL();
    break;
  case Qt::Key_Prior:
    zoom -= 0.2;
    updateGL();
    break;
  case Qt::Key_Next:
    zoom += 0.2;
    updateGL();
    break;
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
  if ( !buf.load( "./data/Star.bmp" ) )
  {
    qWarning( "Could not read image file, using single-color instead." );
    QImage dummy( 128, 128, 32 );
    dummy.fill( Qt::green.rgb() );
    buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat( buf );
  
  glGenTextures( 1, &texture[0] );

  glBindTexture( GL_TEXTURE_2D, texture[0] );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
}
