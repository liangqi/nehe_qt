/*

    Qt OpenGL Tutorial - Lesson 11

    nehewidget.h
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

#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H

#include <qgl.h>
#include <qtimer.h>

class NeHeWidget : public QGLWidget
{
    Q_OBJECT

public:
    
  NeHeWidget( QWidget* parent = 0, const char* name = 0, bool fs = false );
  ~NeHeWidget();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
  
  void keyPressEvent( QKeyEvent *e );
  void loadGLTextures();
  void timerEvent( QTimerEvent * );

protected:

  bool fullscreen;
  
  GLfloat xRot, yRot, zRot;
  GLfloat hold;
  GLuint texture[1];
  
  float points[45][45][3];
  int wiggle_count;
};

#endif//NEHEWIDGET_H
