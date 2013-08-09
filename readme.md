ofxMask
======================
通常のdraw文法でマスク処理ができます。
 
 
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
    
` beginMask(false) ` や ` begin(false) ` を使用すると  
テクスチャをクリアせずに上書きができます。
