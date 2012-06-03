///////////////////////////////////////////////////////////////////////////////
//	Filename:		textwriter.h
//	Author(s):		Craig Niles
//	Date:			6/20/2010
//	
//	Description:	textwriter is used to load a font, generate textures from
//					the glyphs produced by FreeType and render strings using
//					OpenGL.
//	Modified:		
///////////////////////////////////////////////////////////////////////////////
#ifndef TEXTWRITER_H_
#define TEXTWRITER_H_

#include <SDL/SDL_opengl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <string>
#include <vector>
#include <map>

namespace textwriter
{
  struct texture_char
  {
    GLuint handle;
    float advance;
    int left;
    int top;
    int width;
    int height;
    GLuint tex_coor_buffer;
  };

  struct texture_font
  {
    GLuint font_texture;
    int font_texture_size;
    int height;
    std::string fontname;
    std::vector<texture_char> characters;
  };

  namespace {
    std::vector<texture_font> fonts;
    GLuint tex_coor_buffer;
    GLuint vertex_buffer;
    GLuint shader_modelview_loc;
    GLuint shader_projection_loc;
    GLuint shader_color_loc;
    GLuint shader_texture_loc;
    std::string fontname;
    unsigned int current_font;
    int screenw, screenh;
  }
  
  // initialize the textwriter with the shader program pointer and the screen width and height
  bool init_textwriter(int _screenw, int _screenh);
  
  // loads a font from a ttf file.  generates the textures  at specified character size
  unsigned int load_font(std::string filename, std::string fontname, int width, int height);	
	
	// tries to set the current font. if not found, no change are made
	void set_font(unsigned int font);
	
	// gets the height of current font if available. returns 0 otherwise.
	int get_height();

	// get the length of a string in pixels using the currently selected font.
	int get_length(std::string s);

	void set_ortho_mode();

	void unset_ortho_mode();
	void write_text(std::string s, int x, int y, float r, float g, float b, float a);
}
#endif
