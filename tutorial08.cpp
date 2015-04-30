

#include "lsms.h"
#include "draw_lsms.h"


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

int main( void )
{
        srand(930);
        int i = 0;
        lsms * l = lsms_create_from_file("test_spring_1.txt");
	draw_lsms * dl = draw_lsms_create_from_file("test_spring_draw_1.txt");

        lsms_first_step(l, .001);



	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "The Spring mass thingy", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.2f, 0.2f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
//	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texturew	GLuint Texture = loadDDS("uvmap.DDS");
	GLuint Texture = loadDDS("uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	
        
        static GLfloat * g_vertex_buffer_data = new GLfloat[3 * dl->num_draw_vertices];
	static GLfloat * g_uv_buffer_data = new GLfloat[2 * dl->num_draw_vertices];
	static GLfloat * g_normal_buffer_data = new GLfloat[3 * dl->num_draw_vertices];

                for(i = 0; i < dl->num_draw_vertices; i++)
                {
                 //   glm::vec3 pos = 
	            g_vertex_buffer_data[3*i + 0] = (GLfloat)particle_get_x_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 1] = (GLfloat)particle_get_y_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 2] = (GLfloat)particle_get_z_a(l->p[dl->draw_vertices_list[i]]);
                }

                for( i = 0; i < (int)(dl->num_draw_vertices / 3); i++)
                {
		    g_uv_buffer_data[6*i + 0] = 0.000059f;
                    g_uv_buffer_data[6*i + 1] = 0.000004f; 
		    g_uv_buffer_data[6*i + 2] = 0.000103f; 
                    g_uv_buffer_data[6*i + 3] = 0.336048f; 
		    g_uv_buffer_data[6*i + 4] = 0.335973f; 
                    g_uv_buffer_data[6*i + 5] = 0.335903f; 
                }
                
           //     for( i = 0; i < (int)(dl->num_draw_vertices); i++)
           //     {
           //         g_normal_buffer_data[3*i + 0] = 1.0;
           //         g_normal_buffer_data[3*i + 0] = 0.0;
           //         g_normal_buffer_data[3*i + 0] = 0.0;
           //     }

    

        int size = 3 * dl->num_draw_vertices * sizeof(GLfloat);


	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * dl->num_draw_vertices * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * dl->num_draw_vertices * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPos");

        // store values for calc normal
        GLfloat u_x = 0.0;
        GLfloat u_y = 0.0;
        GLfloat u_z = 0.0;
        GLfloat v_x = 0.0;
        GLfloat v_y = 0.0;
        GLfloat v_z = 0.0;
        GLfloat s_x = 0.0;
        GLfloat s_y = 0.0;
        GLfloat s_z = 0.0;


        GLfloat scale = 0.0;

        
        double lastTime = glfwGetTime();
	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
                int num_steps = (int)(delta*10000.0);
                if (num_steps > 100)
                    num_steps = 100;
                

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(2,2,0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

                lsms_update(l, .001, num_steps);

                for(i = 0; i < dl->num_draw_vertices; i++)
                {
	            g_vertex_buffer_data[3*i + 0] = particle_get_x_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 1] = particle_get_y_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 2] = particle_get_z_a(l->p[dl->draw_vertices_list[i]]);
                }

                for( i = 0; i < (int)(dl->num_draw_vertices / 3); i++)
                {
		    g_uv_buffer_data[6*i + 0] = 0.000059f;
                    g_uv_buffer_data[6*i + 1] = 0.000004f; 
		    g_uv_buffer_data[6*i + 2] = 0.000103f; 
                    g_uv_buffer_data[6*i + 3] = 0.336048f; 
		    g_uv_buffer_data[6*i + 4] = 0.335973f; 
                    g_uv_buffer_data[6*i + 5] = 0.335903f; 
                }
                
                
                for( i = 0; i < (int)(dl->num_draw_vertices*3); i++)
                {
                    g_normal_buffer_data[i] = 0.001;
                }

                for( i = 0; i < (int)(dl->num_draw_vertices/3); i++)
                {
                
                    u_x = g_vertex_buffer_data[9*i + 3] - g_vertex_buffer_data[9*i + 0];
                    u_y = g_vertex_buffer_data[9*i + 4] - g_vertex_buffer_data[9*i + 1];
                    u_z = g_vertex_buffer_data[9*i + 5] - g_vertex_buffer_data[9*i + 2];
                    v_x = g_vertex_buffer_data[9*i + 6] - g_vertex_buffer_data[9*i + 0];
                    v_y = g_vertex_buffer_data[9*i + 7] - g_vertex_buffer_data[9*i + 1];
                    v_z = g_vertex_buffer_data[9*i + 8] - g_vertex_buffer_data[9*i + 2];

                    

                    s_x = u_y * v_z - u_z * v_y;
                    s_y = u_z * v_x - u_x * v_z;
                    s_z = u_x * v_y - u_y * v_x;
                    scale = sqrt(pow(s_x,2) + pow(s_y,2) + pow(s_z,2));
                    
                    g_normal_buffer_data[9*i + 0] += (u_y * v_z - u_z * v_y) / scale;
                    g_normal_buffer_data[9*i + 1] += (u_z * v_x - u_x * v_z) / scale;
                    g_normal_buffer_data[9*i + 2] += (u_x * v_y - u_y * v_x) / scale;
                    g_normal_buffer_data[9*i + 3] += (u_y * v_z - u_z * v_y) / scale;
                    g_normal_buffer_data[9*i + 4] += (u_z * v_x - u_x * v_z) / scale;
                    g_normal_buffer_data[9*i + 5] += (u_x * v_y - u_y * v_x) / scale;
                    g_normal_buffer_data[9*i + 6] += (u_y * v_z - u_z * v_y) / scale;
                    g_normal_buffer_data[9*i + 7] += (u_z * v_x - u_x * v_z) / scale;
                    g_normal_buffer_data[9*i + 8] += (u_x * v_y - u_y * v_x) / scale;

                }


                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	        glBufferData(GL_ARRAY_BUFFER, 3 * dl->num_draw_vertices * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, 3*dl->num_draw_vertices * sizeof(GLfloat), g_vertex_buffer_data);

                glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	        glBufferData(GL_ARRAY_BUFFER, 2 * dl->num_draw_vertices * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * dl->num_draw_vertices * sizeof(GLfloat), g_uv_buffer_data);

                glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	        glBufferData(GL_ARRAY_BUFFER, 3* dl->num_draw_vertices * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, 3*dl->num_draw_vertices *sizeof(GLfloat), g_normal_buffer_data);
 

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

                


		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, dl->num_draw_vertices );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

