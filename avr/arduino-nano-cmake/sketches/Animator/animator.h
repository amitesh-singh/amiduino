/*
 * animator.h
 *
 *  Created on: Dec 4, 2016
 *      Author: ami
 */

#ifndef SKETCHES_ANIMATOR_ANIMATOR_H_
#define SKETCHES_ANIMATOR_ANIMATOR_H_
#include <stdint.h>

typedef void (*anim_cb)(void *data);

class animator;
struct animinfo
{
 public:
    animator *_anim;
    anim_cb _anim_cb;
    void *_data;
};
class coreloop
{
 public:
    coreloop(uint8_t fps);

    void loop();
};

class animator
{
 public:
    void add(anim_cb cb, void *data);
    void del(anim_cb, void *data);
};


#endif /* SKETCHES_ANIMATOR_ANIMATOR_H_ */
