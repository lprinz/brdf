/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pointcloud.h"

static const char kVertexShader[] =
    "attribute vec4 vertex;\n"
    "attribute vec3 color;\n"
    "uniform mat4 mvp;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_PointSize = 7.0;\n"
    "  gl_Position = mvp*vertex;\n"
    " float r=color.x + (1.370705f * (color.z-128.0f));\n"
    " float g=color.x - (0.698001f * (color.z-128.0f)) - (0.377633f * (color.y-128.0f));\n"
    " float b=color.x + (1.732446f * (color.y-128.0f));\n"
    "  v_color = vec4(r/255.0f,g/255.0f,b/255.0f,1.0);\n"
// "  v_color = vec4(color.x/256.0f,color.x/256.0f,color.x/256.0f,1.0);\n"
 //g/255.0f,b/255.0f,1.0);\n"
    "}\n";

//TODO changed vertex shader
//"  v_color = vec4(color.x/1280.0f,color.y/720.0f,0.0,1.0);\n"
//   "  v_color = vec4(color.x/255.0,color.y/255.0,color.z/255.0,1.0);\n"

static const char kFragmentShader[] =
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(v_color);\n"
    "}\n";

static const glm::mat4 inverse_z_mat =
    glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

Pointcloud::Pointcloud() {
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  shader_program_ =
      tango_gl::util::CreateProgram(kVertexShader, kFragmentShader);
  if (!shader_program_) {
    LOGE("Could not create program.");
  }
  uniform_mvp_mat_ = glGetUniformLocation(shader_program_, "mvp");
  attrib_vertices_ = glGetAttribLocation(shader_program_, "vertex");
  //TODO
  color_vertices_ = glGetAttribLocation(shader_program_, "color");
  glGenBuffers(1, &vertex_buffers_);
  glGenBuffers(1, &color_buffers_);
}



void Pointcloud::Render(glm::mat4 projection_mat, glm::mat4 view_mat,
                        glm::mat4 model_mat, int depth_buffer_size,
                        float *depth_data_buffer) {
  glUseProgram(shader_program_);

  // Lock xyz_ij mutex.
  pthread_mutex_lock(&TangoData::GetInstance().xyzij_mutex);

  // Calculate model view projection matrix.
  glm::mat4 mvp_mat = projection_mat * view_mat * model_mat * inverse_z_mat;
  glUniformMatrix4fv(uniform_mvp_mat_, 1, GL_FALSE, glm::value_ptr(mvp_mat));

//TODO
  //send MVP to Tango Data
  TangoData::GetInstance().setMVP(&mvp_mat);
    //TODO fill color_buffer


    //if (TangoData::GetInstance().color_buffer == nullptr) {

           /*
        TangoData::GetInstance().color_buffer[3*i]=(float)TangoData::GetInstance().image[3*i];//TangoData::GetInstance().image[i];
        TangoData::GetInstance().color_buffer[3*i+1]=(float)TangoData::GetInstance().image[3*i+1];//TangoData::GetInstance().image[4*i+1]/255.0f;
        TangoData::GetInstance().color_buffer[3*i+2]=(float)TangoData::GetInstance().image[3*i+2];//TangoData::GetInstance().image[4*i+2]/255.0f;
                                                         // e.g. mypixels[x][y] = bPixels[x*width+y] / 255.0
*/
   //float r = TangoData::GetInstance().color_buffer[0];
     //                __android_log_print(ANDROID_LOG_INFO,"fuck", "%f", r);
                     /*
                     float g = TangoData::GetInstance().color_buffer[3*i+1];
                                          __android_log_print(ANDROID_LOG_INFO,"g", "%f", g);
                                          float b = TangoData::GetInstance().color_buffer[3*i+2];
                                                               __android_log_print(ANDROID_LOG_INFO,"b", "%f", b);
*/
           // }

            FILE* file = fopen("/sdcard/hello.txt","w+");

               if (file != NULL)
               {
                 //  fputs("HELLO WORLD!\n", file);
                 //  fflush(file);
                 //  fclose(file);
               }



//TODO bind color buffer

     glBindBuffer(GL_ARRAY_BUFFER, color_buffers_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * depth_buffer_size, TangoData::GetInstance().color_buffer, GL_STATIC_DRAW);
        glEnableVertexAttribArray(color_vertices_);
        glVertexAttribPointer(color_vertices_, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Bind vertex buffer.
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * depth_buffer_size,
               depth_data_buffer, GL_STATIC_DRAW);
  glEnableVertexAttribArray(attrib_vertices_);
  glVertexAttribPointer(attrib_vertices_, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDrawArrays(GL_POINTS, 0, 3 * depth_buffer_size);

  // Unlock xyz_ij mutex.
  pthread_mutex_unlock(&TangoData::GetInstance().xyzij_mutex);
  //TODO
 // pthread_mutex_unlock(&TangoData::GetInstance().image_mutex);


  tango_gl::util::CheckGlError("glDrawArray()");
  glUseProgram(0);
  tango_gl::util::CheckGlError("glUseProgram()");
}
