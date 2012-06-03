///////////////////////////////////////////////////////////////////////////////
//	Filename:		
//	Author(s):		Craig Niles
//	Date:			6/20/2010
//     
//	Description:	
//	Modified:		7/31/201 - Fonts now stored in one texture; access individual
//                          characters through texture coordinates now.
///////////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <cmath>
#include "textwriter.h"
#include "debug.h"

namespace textwriter
{
  bool init_textwriter(int _screenw, int _screenh)
  {
    screenw = _screenw;
    screenh = _screenh;
    fontname = "NULL";
    return false;
  }

  int charRowPos(int c, int cr) {
    return (int)(c / cr);
  }
  int charColPos(int c, int cr) {
    return c % cr;
  }

 unsigned int load_font(std::string filename, std::string fontname, int width, int height)
  {
    int o_width = width;
    int o_height = height;
    std::vector<GLubyte> rgbpixels;
    FT_Library ft_lib;
    FT_Face ft_face;
    unsigned int char_index;
    FT_Glyph ft_glyph;
    FT_BitmapGlyph ft_bmp;
    char ft_pm;
    texture_font new_tf;
    new_tf.characters.resize(256);
    texture_char new_tc;

    if( width == 0 || height == 0 )
      {
	DEBUG_OUT("Can't create a font with width or height of 0.");
	return 0;
      }
    if(FT_Init_FreeType(&ft_lib))
      {
	DEBUG_OUT("Failed initialization of FreeType.");
	return 0;
      }

    if(FT_New_Face(ft_lib, filename.c_str(), 0, &ft_face))
      {
	DEBUG_OUT("Failed to load font file: " + filename);
	return 0;
      }	

    new_tf.font_texture_size = std::sqrt(128 * width * height);
    new_tf.font_texture_size--;
    new_tf.font_texture_size |= new_tf.font_texture_size >> 1;
    new_tf.font_texture_size |= new_tf.font_texture_size >> 2;
    new_tf.font_texture_size |= new_tf.font_texture_size >> 4;
    new_tf.font_texture_size |= new_tf.font_texture_size >> 8;
    new_tf.font_texture_size |= new_tf.font_texture_size >> 16;
    new_tf.font_texture_size++;

    static GLubyte *font_image = new GLubyte[new_tf.font_texture_size * new_tf.font_texture_size * 4];
    
    for(int i = 0; i < new_tf.font_texture_size; i++) {
      for(int j = 0; j < new_tf.font_texture_size; j++) {
	font_image[i*new_tf.font_texture_size+j+0] = 0;
	font_image[i*new_tf.font_texture_size+j+1] = 0;
	font_image[i*new_tf.font_texture_size+j+2] = 0;
	font_image[i*new_tf.font_texture_size+j+3] = 0;
      }
    }

    glGenTextures(1,&new_tf.font_texture);
    glBindTexture(GL_TEXTURE_2D,new_tf.font_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, new_tf.font_texture_size,new_tf.font_texture_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_image);
    delete[](font_image);

    width = width * 64;
    height = height * 64;

    FT_Set_Char_Size(ft_face, width, height, 0, 0);
    
    for(unsigned char i = 0; i < 128; i++)
      {
	char_index = FT_Get_Char_Index(ft_face, i);
	if(char_index == 0)
	  {
	    continue;
	  }
	if(FT_Load_Glyph(ft_face, char_index, FT_LOAD_DEFAULT))
	  {
	    continue;
	  }
	if(FT_Get_Glyph(ft_face->glyph, &ft_glyph))
	  {
	    continue;
	  }
	if(FT_Glyph_To_Bitmap(&ft_glyph, ft_render_mode_normal, 0, true))
	  {
	    continue;
	  }

	ft_bmp = (FT_BitmapGlyph)ft_glyph;

	new_tc.width = (ft_bmp->bitmap.width)? ft_bmp->bitmap.width : 16;
	new_tc.height = (ft_bmp->bitmap.rows)? ft_bmp->bitmap.rows: 16;
	//std::cerr << "tc w/h: " << new_tc.width << " " << new_tc.height << std::endl;
	new_tc.left = ft_bmp->left;
	new_tc.top = ft_bmp->top;
	new_tc.advance = ft_bmp->root.advance.x>>16;

	rgbpixels.clear();
	rgbpixels.resize(new_tc.width * new_tc.height * 4);
	for(int j = 0; j < rgbpixels.size()/4; j++)
	  {
	    if(ft_bmp->bitmap.buffer)
	      {
		rgbpixels[j*4] = ((unsigned char*)ft_bmp->bitmap.buffer)[j];
		rgbpixels[j*4+1] = ((unsigned char*)ft_bmp->bitmap.buffer)[j];
		rgbpixels[j*4+2] = ((unsigned char*)ft_bmp->bitmap.buffer)[j];
		rgbpixels[j*4+3] = ((unsigned char*)ft_bmp->bitmap.buffer)[j];
	      }
	    else
	      {
		rgbpixels[j*4] = 0;
		rgbpixels[j*4+1] = 0;
		rgbpixels[j*4+2] = 0;
		rgbpixels[j*4+3] = 0;
	      }
	  }

	/*glGenTextures(1,&new_tc.handle);
	glBindTexture(GL_TEXTURE_2D,new_tc.handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, new_tc.width,new_tc.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rgbpixels[0]);*/

	int cr = int(new_tf.font_texture_size / o_width);
	GLint x_offset = charColPos(i, cr) * o_width;
	GLint y_offset = charRowPos(i, cr) * o_height;

	glBindTexture(GL_TEXTURE_2D, new_tf.font_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, new_tc.width, new_tc.height, GL_RGBA, GL_UNSIGNED_BYTE, &rgbpixels[0]);

	glGenBuffers(1, &new_tc.tex_coor_buffer);
 	glBindBuffer(GL_ARRAY_BUFFER, new_tc.tex_coor_buffer);
	GLfloat size = new_tf.font_texture_size;

	GLfloat tex_coors[8];
	tex_coors[0] = x_offset / size;
	tex_coors[1] = (y_offset + new_tc.height) / size;
	tex_coors[2] = (x_offset + new_tc.width) / size;
	tex_coors[3] = (y_offset + new_tc.height) / size;
	tex_coors[4] = (x_offset + new_tc.width) / size;
	tex_coors[5] = y_offset / size;
	tex_coors[6] = x_offset / size;
	tex_coors[7] = y_offset / size;
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, tex_coors, GL_DYNAMIC_DRAW);

	new_tf.characters[(char)i] = new_tc;
	FT_Done_Glyph(ft_glyph);
      }

    new_tf.height = height/64;

    fonts.push_back(new_tf);

    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_lib);

    glGenBuffers(1, &vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    GLfloat vertices[] = 
      {0.0f, 0.0f,
       1.0f, 0.0f,
       1.0f, 1.0f,
       0.0f, 1.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return fonts.size();
  }

  void set_ortho_mode()
  {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenw, 0, screenh, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
  }

  void unset_ortho_mode()
  {
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }

  void set_font(unsigned int font)
  {
    current_font = font-1;
  }

  int get_height()
  {
    if(current_font != -1)
      {
	return fonts[current_font].height;
      }
    return 0;
  }

  int get_length(std::string s)
  {
    if(current_font != -1)
      {			
	int len = 0;
	for(int i = 0; i < s.size(); i++)
	  {
	    len += fonts[current_font].characters[s[i]].advance;
	  }
	return len;
      }
    return 0;
  }

  void write_text(std::string s, int x, int y, float r, float g, float b, float a)
  {
    static float modelview[16];
    static float projection[16];

    if(current_font == -1)
      {
	return;
      }


    set_ortho_mode();
    texture_font *tf = &fonts[current_font];
    texture_char *tc;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTranslatef(x, y, 0);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    glBindTexture(GL_TEXTURE_2D, tf->font_texture);

    for( int i = 0; i < s.size(); i++)
      {
	tc = &tf->characters[s[i]];
	glPushMatrix();
	glTranslatef(tc->left, tc->top - tc->height, 0.0f);
	glScalef(tc->width, tc->height, 0.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glUniformMatrix4fv(shader_modelview_loc, 1, GL_FALSE, modelview);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexPointer(2, GL_FLOAT, 0, (char*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tc->tex_coor_buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL);

	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glTranslatef(tc->advance, 0, 0);
      }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    unset_ortho_mode();
  }
}
