#ifndef DEFORM_H
#define DEFORM_H

#include "Effects/Effect.h"
#include "Assets/Sprite.h"

/*

    Deform
    ------
    LUT based deformation effect.

    Constructor can be either with a sprite specified or will fallback
    to default with a XOR generated texture.

    Reference: http://iquilezles.org/www/articles/deform/deform.htm

*/

namespace Effects
{
    class Deform : public Effects::Effect
    {
        private:
            Assets::Sprite *asset_ = 0; // Pointer to texture asset
            unsigned char *text_buff_c_ = 0; // Store texture characters here
            unsigned char *text_buff_a_ = 0; // Store texture colors here
            char *lut_u_ = 0; // Store look up table texture U
            char *lut_v_ = 0; // Store look up table texture V
            unsigned char texture_width_; // Texture width
            unsigned char texture_height_; // Texture height
            unsigned char screen_width_; // Screen width
            unsigned char screen_height_; // Screen height
            unsigned char pixel_, color_; // This is to avoid creating variables for each loop
            int index_, u_, v_, val_; // This is to avoid creating variables for each loop
            double speed_; // Effect speed

            // XOR texture generator
            void XorTexture(unsigned char width, unsigned char height, unsigned char type = 0) {
                // Store texture width and height
                texture_width_ = width;
                texture_height_ = height;
                for (unsigned int y = 0; y < texture_height_; y++)
                    for (unsigned int x = 0; x < texture_width_; x++) {
                        switch(type) {
                            default:
                            case 0:
                                // Set character
                                text_buff_c_[x+(texture_width_*y)] = 32;
                                break;
                            case 1:
                                text_buff_c_[x+(texture_width_*y)] = 0x20 + y % 0x5F;
                                break;
                        }
                        // Set color
                        GenerateColor(x, y, type);
                    }
            }

            // Generate colors depending on type
            void GenerateColor(unsigned int x, unsigned int y, unsigned char type = 0) {
                // Custom color palette
                unsigned char color[13] = {0x00, 0x08, 0x07, 0x0F, 0x80, 0x88, 0x87, 0x70, 0x78, 0x77, 0xF0, 0xF7, 0xFF};
                switch(type) {
                    default:
                    case 0:
                        // Set color
                        text_buff_a_[x+(texture_width_*y)] = color[(x+y) % sizeof(color)];
                        break;
                    case 1:
                        text_buff_a_[x+(texture_width_*y)] = color[x % sizeof(color)];
                        break;
                    case 2:
                        text_buff_a_[x+(texture_width_*y)] = 0x70 + (x^y) % 0x20;
                        break;
                    case 3:
                        text_buff_a_[x+(texture_width_*y)] = (0x20 * x) % 0xEF;
                        break;
                    case 4:
                        text_buff_a_[x+(texture_width_*y)] = 0x70 + x % 0x1F;
                        break;
                }
            }

            // Look Up Table generator
            void GenerateLUT(unsigned char type = 0) {
                float d,a,u,v,i,j;
                float W = (float)screen_width_;
                float H = (float)screen_height_;
                float halfW = W*0.5;
                float halfH = H*0.5;
                double PI = 3.14159265359;
                for( int y=0; y < screen_height_; y++ )
                    for( int x=0; x < screen_width_; x++ ) {
                        i = -1.00f + 2.00f * x / W;
                        j = -1.00f + 2.00f * y / H;
                        d = sqrtf(i*i + j*j);
                        a = atan2f(j, i);
                        // Deformation presets
                        switch(type) {
                            default:
                            case PLANE:
                                // Scrolling distorted plane
                                u = d*cos(a+d);
                                v = d*sin(a+d);
                                break;
                            case FLOWER:
                                // Flower effect
                                u = 0.35*cosf(a*PI)/d*PI;
                                v = 0.35*sinf(a*PI)/d*PI;
                                break;
                            case TUNNEL:
                                // Distorted Tunnel
                                u = 2.0 * (texture_height_/sqrtf((x-halfW)*(x-halfW)+(y-halfH)*(y-halfH)));
                                v = sinf(a*PI)/d*0.15;
                                break;
                            case PLASMA:
                                // Plasma
                                u = halfW + (halfW * sinf(x/12.0) * 0.03);
                                v = halfH + (halfH * cosf(y/12.0) * 0.035);
                                /*
                                u = halfW + (sinf(x/8.0)/(1.0+cosf(y)*0.05) );
                                v = halfH + (cosf(y/8.0)/(1.0+sinf(x)*0.05) );
                                */
                                break;
                            case SPIDER:
                                // Spider
                                u = i*cos(360) - j*sin(360)+cosf(a)/d;
                                v = j*cos(360) + i*sin(360)-sinf(a)/d;
                                break;
                        }
                        // LUT U
                        lut_u_[x+(screen_width_*y)] = (static_cast<int>(texture_width_*u)) & (texture_width_-1);
                        // LUT V
                        lut_v_[x+(screen_width_*y)] = (static_cast<int>(texture_height_*v)) & (texture_height_-1);
                    }
            }

        public:
            Deform(ConsoleEngine *engine, const char *path = 0,
                   float speed = 20.0f, unsigned char type = 0,
                   unsigned char coltype = 0) : Effects::Effect(engine, speed) {
                // Set screen width and height var and allocated memory for LUT buffers
                screen_width_ = engine_->width();
                screen_height_ = engine_->height();
                // Allocate memory for look up tables U and V
                lut_u_ = new char[screen_width_*screen_height_];
                lut_v_ = new char[screen_width_*screen_height_];
                // If path is set, check if asset already loaded, if not load and store it
                if (path != NULL && !(asset_ = engine_->asset_manager()->Get<Assets::Sprite>(path))) {
                    LOG->info("Asset %s loaded and stored from effect", path);
                    engine_->asset_manager()->Load<Assets::Sprite>(path);
                    asset_ = engine_->asset_manager()->Get<Assets::Sprite>(path);
                }
                // If Asset is stored properly use it
                if (asset_ != NULL) {
                    // Store texture width and height from asset properties
                    texture_width_ = asset_->w();
                    texture_height_ = asset_->h();
                    // Allocate memory for texture buffers (character and attribute)
                    text_buff_c_ = new unsigned char[texture_width_*texture_height_];
                    text_buff_a_ = new unsigned char[texture_width_*texture_height_];
                    for (unsigned char y = 0; y < texture_height_; y++) {
                        for (unsigned char x = 0; x < texture_width_; x++) {
                            text_buff_c_[x+(texture_width_*y)] = asset_->data()->at(x+(texture_width_*y));
                            // Set color
                            GenerateColor(x, y, coltype);
                        }
                    }
                } else {
                    // No Asset has been specified or loading failed, use XOR texture as fallback
                    texture_width_ = 32;
                    texture_height_ = 32;
                    // Allocate memory for texture buffers (character and attribute)
                    text_buff_c_ = new unsigned char[texture_width_*texture_height_];
                    text_buff_a_ = new unsigned char[texture_width_*texture_height_];
                    XorTexture(texture_width_, texture_height_, coltype);
                }
                // Generate LUT table
                GenerateLUT(type);
            }
            ~Deform() {
                // Clean allocated buffers for char and color
                delete[] text_buff_c_;
                delete[] text_buff_a_;
                // Clear allocated LUT buffers
                delete[] lut_u_;
                delete[] lut_v_;
            }

            // Enum of deformation types
            enum deformType {
                PLANE,
                TUNNEL,
                FLOWER,
                SPIDER,
                PLASMA
            };
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method
            // ----------------------------------------------------------------------------
            void Draw() {
                // Calculate speed
                speed_ = speed() * engine_->time_elapsed();
                // Draw Deform by picking values from the LUT
                for (unsigned char y = 0; y < screen_height_; y++)
                    for (unsigned char x = 0; x < screen_width_; x++) {
                        // Get value from LUT
                        index_ = (x+(screen_width_*y));
                        // Get LUT U
                        u_ = lut_u_[index_] + speed_;
                        // Get LUT V
                        v_ = lut_v_[index_] + speed_;
                        // Set texture pixel value based on U and V
                        val_ = u_%(texture_width_-1) + (texture_width_*(v_&(texture_height_-1)));
                        // Show only texture - for debugging
                        //val = (x%(texture_width_-1)) + (texture_width_*(y&(texture_height_-1)));
                        // Make sure we don't have negative val
                        if (val_ < 0) val_ = 0;
                        // Get pixel and color from texture buffer using U and V
                        pixel_ = text_buff_c_[val_];
                        color_ = text_buff_a_[val_];
                        // If not transparent put pixel
                        if (pixel_ < 255)
                            engine_->PutPixel(x, y, pixel_, color_);
                    }
            }
    };
}


#endif // DEFORM_H
