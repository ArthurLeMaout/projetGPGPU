#test branch
#include <iostream>
#include <fstream>
#include <chrono>


#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "external/tiny_obj_loader.h"

// main obj
GLuint program_id;
GLuint VAO;
GLuint n_elements;
GLuint texture_id;

// camera
Camera cam;

void init()
{
  // create main obj
  {
    std::cout << "Creation of main object" << std::endl; 
    program_id = glhelper::create_program_from_file("shaders/simple.vs", "shaders/simple.fs");
    Mesh m = Mesh::load_from_file("data/Frankie/Frankie3.obj");
    m.apply_matrix(glm::mat4(
          1., 0., 0., 0.,
          0., 1., 0., 0.,
          0., 0., 1., 0.,
          0., 0., 0., 1.));
    n_elements=m.size_element();
    VAO = m.load_to_gpu();
    texture_id = glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");
  }

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glEnable(GL_DEPTH_TEST);            
}

static void display_callback()
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  glm::mat4 mvp = cam.projection() * cam.view();

  // display obj
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, cam.width(), cam.height());
    glUseProgram(program_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBindVertexArray(VAO);
    GLint mvp_id = glGetUniformLocation(program_id, "MVP");
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]); 
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0); 
  }

  glBindVertexArray(0);
  glutSwapBuffers ();
}

static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
  glutPostRedisplay();
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);

  glViewport(0,0, width, height); 
  glutPostRedisplay();
}


static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);

  glutPostRedisplay();
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
  glutPostRedisplay();
}

static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInitContextVersion(3, 3); 
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(cam.width(), cam.height());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewExperimental=true;
  glewInit();

  init();
  glutMainLoop();

  return 0;
}
