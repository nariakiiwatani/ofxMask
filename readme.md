ofxMask
======================
an easy way to mask


how to use
------
    ofxMask mask;

    // creating masker texture
    mask.beginMask();
    // draw masker here in gray scale
    ofSetColor(255);
    ofRect(100, 200, 100, 50);
    ofSetColor(128);    // more black, more transparent
    ofEllipse(250, 100, 150, 150);
    mask.endMask();

    // creating maskee texture
    mask.begin();
    // draw maskee here
    sometexture.draw();
    mask.end();

    // draw result
    mask.draw();

use ` beginMask(false) ` and ` begin(false) ` not to clear textures.
