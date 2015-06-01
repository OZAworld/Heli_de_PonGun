xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 24;
 -12.44320;56.16520;-112.00000;,
 12.44570;56.16520;-112.00000;,
 7.96520;58.30700;-7.96440;,
 -7.96370;58.30700;-7.96440;,
 -111.99880;56.16520;-12.44440;,
 -7.96370;58.30700;7.96440;,
 -111.99880;56.16520;12.44440;,
 7.96520;58.30700;7.96440;,
 12.44560;56.16520;112.00000;,
 -12.44320;56.16520;112.00000;,
 1.12490;39.37800;1.42750;,
 -0.96860;39.37800;1.42750;,
 -0.96860;56.16520;1.42750;,
 1.12490;56.16520;1.42750;,
 1.12490;39.37800;-0.66600;,
 1.12490;56.16520;-0.66600;,
 -0.96860;39.37800;-0.66600;,
 -0.96860;56.16520;-0.66600;,
 112.00120;56.16520;-12.44440;,
 112.00120;56.16520;12.44440;,
 7.96520;56.16520;7.96440;,
 -7.96370;56.16520;7.96440;,
 7.96520;56.16520;-7.96440;,
 -7.96370;56.16520;-7.96440;;
 
 18;
 4;3,2,1,0;,
 4;6,5,3,4;,
 4;9,8,7,5;,
 4;13,12,11,10;,
 4;15,13,10,14;,
 4;17,15,14,16;,
 4;12,17,16,11;,
 4;16,14,10,11;,
 4;2,7,19,18;,
 4;3,5,7,2;,
 3;20,7,8;,
 3;5,21,9;,
 3;7,20,19;,
 3;22,2,18;,
 3;2,22,1;,
 3;23,3,0;,
 3;3,23,4;,
 3;21,5,6;;
 
 MeshMaterialList {
  2;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.725000;0.725000;0.725000;1.000000;;
   15.000000;
   0.270000;0.270000;0.270000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Prop.bmp";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshTextureCoords {
  24;
  -0.555550;0.000000;
  -0.444440;0.000000;
  -0.464450;0.464440;
  -0.535560;0.464440;
  -1.000000;0.444440;
  -0.535560;0.535560;
  -1.000000;0.555550;
  -0.464450;0.535560;
  -0.444450;1.000000;
  -0.555560;1.000000;
  -0.494980;0.506370;
  -0.504330;0.506370;
  -0.504330;0.506370;
  -0.494980;0.506370;
  -0.494980;0.497030;
  -0.494980;0.497030;
  -0.504330;0.497030;
  -0.504330;0.497030;
  0.000000;0.444450;
  -0.000000;0.555560;
  -0.464450;0.535560;
  -0.535560;0.535560;
  -0.464450;0.464440;
  -0.535560;0.464440;;
 }
}
