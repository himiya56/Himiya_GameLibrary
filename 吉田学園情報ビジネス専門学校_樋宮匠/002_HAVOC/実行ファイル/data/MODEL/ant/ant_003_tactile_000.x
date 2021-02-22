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
 176;
 16.78500;20.71640;-20.36680;,
 7.57020;9.91220;-11.41000;,
 6.58440;11.84120;-10.93780;,
 15.79920;22.64600;-19.89460;,
 6.62760;13.37420;-9.48580;,
 15.84300;24.17900;-18.44320;,
 7.68060;13.79420;-7.73260;,
 16.89600;24.59900;-16.69000;,
 9.25020;12.90440;-6.49900;,
 18.46560;23.70860;-15.45640;,
 10.60200;11.12120;-6.36280;,
 19.81680;21.92540;-15.31900;,
 11.10360;9.27920;-7.38580;,
 20.31840;20.08400;-16.34320;,
 10.51980;8.24000;-9.09100;,
 19.73460;19.04480;-18.04900;,
 9.12420;8.48960;-10.68040;,
 18.33960;19.29440;-19.63780;,
 7.57020;9.91220;-11.41000;,
 16.78500;20.71640;-20.36680;,
 -1.64460;-0.89260;-2.45260;,
 -2.63100;1.03640;-1.98040;,
 -2.58720;2.56940;-0.52780;,
 -1.53420;2.99000;1.22480;,
 0.03540;2.10020;2.45840;,
 1.38720;0.31700;2.59520;,
 1.88880;-1.52500;1.57160;,
 1.30500;-2.56420;-0.13420;,
 -0.09060;-2.31400;-1.72300;,
 -1.64460;-0.89260;-2.45260;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 18.00000;21.80000;-17.80000;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 -0.43020;0.19040;0.11480;,
 22.99680;26.00120;-29.94100;,
 26.02680;23.48180;-34.87180;,
 24.48840;22.81880;-34.60840;,
 22.99680;26.00120;-29.94100;,
 22.58520;22.94660;-33.00700;,
 22.99680;26.00120;-29.94100;,
 20.82780;23.83160;-30.49540;,
 22.99680;26.00120;-29.94100;,
 19.68720;25.23740;-27.74740;,
 22.99680;26.00120;-29.94100;,
 19.46820;26.78600;-25.49920;,
 22.99680;26.00120;-29.94100;,
 20.23020;28.06280;-24.35320;,
 22.99680;26.00120;-29.94100;,
 21.76860;28.72640;-24.61480;,
 22.99680;26.00120;-29.94100;,
 23.67120;28.59860;-26.21680;,
 22.99680;26.00120;-29.94100;,
 25.42860;27.71360;-28.72780;,
 22.99680;26.00120;-29.94100;,
 26.56980;26.30780;-31.47580;,
 22.99680;26.00120;-29.94100;,
 26.78880;24.75920;-33.72460;,
 22.99680;26.00120;-29.94100;,
 26.02680;23.48180;-34.87180;,
 28.95480;20.81180;-38.54920;,
 26.06340;19.56500;-38.05600;,
 22.48740;19.80560;-35.04580;,
 19.18440;21.46940;-30.32680;,
 17.04000;24.11060;-25.16140;,
 16.62900;27.02120;-20.93560;,
 18.06120;29.42120;-18.78100;,
 20.95260;30.66860;-19.27420;,
 24.52860;30.42800;-22.28440;,
 27.83160;28.76360;-27.00400;,
 29.97540;26.12300;-32.16820;,
 30.38700;23.21240;-36.39460;,
 28.95480;20.81180;-38.54920;,
 31.42800;18.31220;-40.53160;,
 27.53280;16.63160;-39.86620;,
 22.71420;16.95620;-35.81080;,
 18.26460;19.19780;-29.45200;,
 15.37560;22.75520;-22.49380;,
 14.82180;26.67740;-16.80100;,
 16.75080;29.91080;-13.89700;,
 20.64660;31.59140;-14.56240;,
 25.46520;31.26680;-18.61720;,
 29.91480;29.02520;-24.97660;,
 32.80320;25.46720;-31.93480;,
 33.35760;21.54560;-37.62820;,
 31.42800;18.31220;-40.53160;,
 33.14820;16.28360;-40.57900;,
 28.71840;14.37260;-39.82240;,
 23.23920;14.74160;-35.21080;,
 18.17880;17.29100;-27.97960;,
 14.89380;21.33680;-20.06740;,
 14.26380;25.79600;-13.59280;,
 16.45800;29.47400;-10.29160;,
 20.88780;31.38440;-11.04700;,
 26.36640;31.01540;-15.65920;,
 31.42680;28.46660;-22.88980;,
 34.71180;24.42020;-30.80260;,
 35.34180;19.96160;-37.27720;,
 33.14820;16.28360;-40.57900;,
 33.90720;14.97140;-38.68480;,
 29.47740;13.06100;-37.92940;,
 23.99820;13.42940;-33.31780;,
 18.93840;15.97880;-26.08660;,
 15.65340;20.02460;-18.17380;,
 15.02280;24.48380;-11.69920;,
 17.21700;28.16180;-8.39740;,
 21.64680;30.07220;-9.15400;,
 27.12600;29.70320;-13.76560;,
 32.18580;27.15440;-20.99620;,
 35.47140;23.10800;-28.90900;,
 36.10140;18.64880;-35.38360;,
 33.90720;14.97140;-38.68480;,
 33.61380;14.53400;-35.07880;,
 29.71860;12.85340;-34.41340;,
 24.90000;13.17800;-30.35920;,
 20.45040;15.41960;-23.99980;,
 17.56140;18.97760;-17.04160;,
 17.00760;22.89920;-11.34820;,
 18.93660;26.13320;-8.44480;,
 22.83240;27.81320;-9.10960;,
 27.65100;27.48860;-13.16560;,
 32.10000;25.24700;-19.52380;,
 34.98900;21.68900;-26.48260;,
 35.54340;17.76800;-32.17600;,
 33.61380;14.53400;-35.07880;,
 32.30400;15.02360;-30.19540;,
 29.41260;13.77740;-29.70220;,
 25.83660;14.01740;-26.69140;,
 22.53360;15.68060;-21.97240;,
 20.38920;18.32180;-16.80820;,
 19.97820;21.23240;-12.58240;,
 21.40980;23.63360;-10.42660;,
 24.30120;24.87980;-10.92100;,
 27.87780;24.63920;-13.93000;,
 31.18020;22.97540;-18.65020;,
 33.32460;20.33420;-23.81500;,
 33.73560;17.42360;-28.04080;,
 32.30400;15.02360;-30.19540;,
 30.13500;16.38200;-24.62380;,
 28.59600;15.71840;-24.36160;,
 26.69340;15.84620;-22.75960;,
 24.93600;16.73180;-20.24860;,
 23.79480;18.13700;-17.50060;,
 23.57640;19.68560;-15.25180;,
 24.33840;20.96300;-14.10520;,
 25.87680;21.62600;-14.36800;,
 27.77940;21.49820;-15.96940;,
 29.53680;20.61260;-18.48100;,
 30.67800;19.20740;-21.22900;,
 30.89700;17.65880;-23.47780;,
 30.13500;16.38200;-24.62380;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;,
 27.36840;18.44420;-19.03600;;
 
 144;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;1,20,21,2;,
 4;2,21,22,4;,
 4;4,22,23,6;,
 4;6,23,24,8;,
 4;8,24,25,10;,
 4;10,25,26,12;,
 4;12,26,27,14;,
 4;14,27,28,16;,
 4;16,28,29,18;,
 3;30,0,3;,
 3;31,3,5;,
 3;32,5,7;,
 3;33,7,9;,
 3;34,9,11;,
 3;35,11,13;,
 3;36,13,15;,
 3;37,15,17;,
 3;38,17,19;,
 3;39,21,20;,
 3;40,22,21;,
 3;41,23,22;,
 3;42,24,23;,
 3;43,25,24;,
 3;44,26,25;,
 3;45,27,26;,
 3;46,28,27;,
 3;47,29,28;,
 3;48,49,50;,
 3;51,50,52;,
 3;53,52,54;,
 3;55,54,56;,
 3;57,56,58;,
 3;59,58,60;,
 3;61,60,62;,
 3;63,62,64;,
 3;65,64,66;,
 3;67,66,68;,
 3;69,68,70;,
 3;71,70,72;,
 4;49,73,74,50;,
 4;50,74,75,52;,
 4;52,75,76,54;,
 4;54,76,77,56;,
 4;56,77,78,58;,
 4;58,78,79,60;,
 4;60,79,80,62;,
 4;62,80,81,64;,
 4;64,81,82,66;,
 4;66,82,83,68;,
 4;68,83,84,70;,
 4;70,84,85,72;,
 4;73,86,87,74;,
 4;74,87,88,75;,
 4;75,88,89,76;,
 4;76,89,90,77;,
 4;77,90,91,78;,
 4;78,91,92,79;,
 4;79,92,93,80;,
 4;80,93,94,81;,
 4;81,94,95,82;,
 4;82,95,96,83;,
 4;83,96,97,84;,
 4;84,97,98,85;,
 4;86,99,100,87;,
 4;87,100,101,88;,
 4;88,101,102,89;,
 4;89,102,103,90;,
 4;90,103,104,91;,
 4;91,104,105,92;,
 4;92,105,106,93;,
 4;93,106,107,94;,
 4;94,107,108,95;,
 4;95,108,109,96;,
 4;96,109,110,97;,
 4;97,110,111,98;,
 4;99,112,113,100;,
 4;100,113,114,101;,
 4;101,114,115,102;,
 4;102,115,116,103;,
 4;103,116,117,104;,
 4;104,117,118,105;,
 4;105,118,119,106;,
 4;106,119,120,107;,
 4;107,120,121,108;,
 4;108,121,122,109;,
 4;109,122,123,110;,
 4;110,123,124,111;,
 4;112,125,126,113;,
 4;113,126,127,114;,
 4;114,127,128,115;,
 4;115,128,129,116;,
 4;116,129,130,117;,
 4;117,130,131,118;,
 4;118,131,132,119;,
 4;119,132,133,120;,
 4;120,133,134,121;,
 4;121,134,135,122;,
 4;122,135,136,123;,
 4;123,136,137,124;,
 4;125,138,139,126;,
 4;126,139,140,127;,
 4;127,140,141,128;,
 4;128,141,142,129;,
 4;129,142,143,130;,
 4;130,143,144,131;,
 4;131,144,145,132;,
 4;132,145,146,133;,
 4;133,146,147,134;,
 4;134,147,148,135;,
 4;135,148,149,136;,
 4;136,149,150,137;,
 4;138,151,152,139;,
 4;139,152,153,140;,
 4;140,153,154,141;,
 4;141,154,155,142;,
 4;142,155,156,143;,
 4;143,156,157,144;,
 4;144,157,158,145;,
 4;145,158,159,146;,
 4;146,159,160,147;,
 4;147,160,161,148;,
 4;148,161,162,149;,
 4;149,162,163,150;,
 3;151,164,152;,
 3;152,165,153;,
 3;153,166,154;,
 3;154,167,155;,
 3;155,168,156;,
 3;156,169,157;,
 3;157,170,158;,
 3;158,171,159;,
 3;159,172,160;,
 3;160,173,161;,
 3;161,174,162;,
 3;162,175,163;;
 
 MeshMaterialList {
  3;
  144;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.006400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  146;
  0.693614;0.484837;-0.532760;,
  -0.394906;-0.362887;-0.844016;,
  -0.770734;0.142522;-0.621013;,
  -0.794239;0.600479;-0.092785;,
  -0.424608;0.764393;0.485193;,
  0.130297;0.564560;0.815043;,
  0.616305;0.119631;0.778368;,
  0.835118;-0.394147;0.383701;,
  0.650031;-0.729841;-0.211641;,
  0.152843;-0.704723;-0.692823;,
  -0.394863;-0.362938;-0.844014;,
  -0.770739;0.142519;-0.621007;,
  -0.794240;0.600480;-0.092774;,
  -0.424639;0.764394;0.485163;,
  0.130300;0.564573;0.815033;,
  0.616313;0.119651;0.778358;,
  0.835118;-0.394129;0.383720;,
  0.650002;-0.729858;-0.211670;,
  0.152836;-0.704730;-0.692818;,
  -0.394820;-0.362990;-0.844011;,
  -0.770744;0.142517;-0.621001;,
  -0.794240;0.600481;-0.092763;,
  -0.424669;0.764396;0.485133;,
  0.130304;0.564586;0.815024;,
  0.616321;0.119670;0.778349;,
  0.835117;-0.394111;0.383738;,
  0.649974;-0.729875;-0.211699;,
  0.152829;-0.704737;-0.692812;,
  -0.693526;-0.484921;0.532797;,
  -0.409044;0.731914;-0.544963;,
  -0.329906;0.685473;-0.649067;,
  -0.449377;0.604938;-0.657351;,
  -0.549015;0.553646;-0.626146;,
  -0.605333;0.551202;-0.574237;,
  -0.610835;0.597378;-0.519634;,
  -0.564545;0.676526;-0.472866;,
  -0.473985;0.762223;-0.440856;,
  -0.361947;0.827395;-0.429432;,
  -0.259825;0.858328;-0.442452;,
  -0.194243;0.855447;-0.480084;,
  -0.182007;0.823443;-0.537415;,
  -0.230113;0.764952;-0.601578;,
  -0.211925;0.596897;-0.773823;,
  -0.478161;0.405334;-0.779145;,
  -0.675096;0.289652;-0.678489;,
  -0.776546;0.293194;-0.557686;,
  -0.795704;0.401593;-0.453407;,
  -0.721432;0.585429;-0.369876;,
  -0.541245;0.781473;-0.310411;,
  -0.300114;0.910009;-0.286035;,
  -0.084593;0.947962;-0.306941;,
  0.051660;0.925340;-0.375602;,
  0.089370;0.865962;-0.492060;,
  0.005314;0.763307;-0.646014;,
  0.021117;0.372068;-0.927965;,
  -0.453256;0.028942;-0.890911;,
  -0.746722;-0.106682;-0.656525;,
  -0.883581;-0.059799;-0.464445;,
  -0.940446;0.115781;-0.319617;,
  -0.884952;0.422234;-0.196412;,
  -0.619072;0.778900;-0.100322;,
  -0.204545;0.976421;-0.069018;,
  0.136323;0.984187;-0.113100;,
  0.336410;0.916843;-0.215004;,
  0.421003;0.822411;-0.382619;,
  0.358202;0.670350;-0.649863;,
  0.483627;0.078952;-0.871706;,
  -0.264683;-0.572348;-0.776119;,
  -0.663736;-0.577331;-0.475546;,
  -0.846091;-0.460275;-0.268843;,
  -0.960547;-0.261734;-0.094047;,
  -0.987722;0.104941;0.115726;,
  -0.672236;0.665550;0.324257;,
  -0.026123;0.949152;0.313731;,
  0.411840;0.896012;0.165985;,
  0.633418;0.773706;0.012679;,
  0.762948;0.621986;-0.176190;,
  0.797209;0.357605;-0.486391;,
  0.672238;-0.665541;-0.324269;,
  0.026163;-0.949141;-0.313763;,
  -0.411868;-0.895992;-0.166020;,
  -0.633460;-0.773671;-0.012703;,
  -0.762977;-0.621962;0.176152;,
  -0.797256;-0.357537;0.486364;,
  -0.449442;0.173067;0.876384;,
  0.264692;0.572368;0.776101;,
  0.663732;0.577355;0.475522;,
  0.846103;0.460231;0.268881;,
  0.960561;0.261670;0.094087;,
  0.987722;-0.104941;-0.115725;,
  0.619056;-0.778911;0.100337;,
  0.204572;-0.976414;0.069041;,
  -0.136286;-0.984193;0.113095;,
  -0.336475;-0.916833;0.214947;,
  -0.421059;-0.822389;0.382604;,
  -0.358223;-0.670326;0.649876;,
  -0.021152;-0.372070;0.927964;,
  0.453228;-0.028968;0.890924;,
  0.746731;0.106625;0.656525;,
  0.883581;0.059698;0.464457;,
  0.940415;-0.115878;0.319674;,
  0.884931;-0.422261;0.196448;,
  0.541216;-0.781483;0.310436;,
  0.300097;-0.910007;0.286059;,
  0.084704;-0.947944;0.306964;,
  -0.051578;-0.925336;0.375623;,
  -0.089328;-0.865951;0.492088;,
  -0.005299;-0.763290;0.646034;,
  0.211924;-0.596900;0.773820;,
  0.478112;-0.405379;0.779151;,
  0.675080;-0.289710;0.678480;,
  0.776570;-0.293183;0.557658;,
  0.795713;-0.401577;0.453406;,
  0.721457;-0.585405;0.369865;,
  0.473948;-0.762230;0.440883;,
  0.361939;-0.827396;0.429438;,
  0.259877;-0.858283;0.442509;,
  0.194352;-0.855403;0.480118;,
  0.182123;-0.823439;0.537382;,
  0.230225;-0.764917;0.601580;,
  0.329926;-0.685470;0.649060;,
  0.449329;-0.604996;0.657330;,
  0.548979;-0.553691;0.626138;,
  0.605286;-0.551241;0.574250;,
  0.610793;-0.597433;0.519620;,
  0.564502;-0.676566;0.472860;,
  0.409015;-0.731941;0.544949;,
  0.693496;0.484813;-0.532936;,
  0.693550;0.484742;-0.532929;,
  0.693582;0.484798;-0.532837;,
  0.693648;0.484848;-0.532706;,
  0.693759;0.484777;-0.532625;,
  0.693741;0.484911;-0.532526;,
  0.693633;0.484972;-0.532611;,
  0.693602;0.484824;-0.532786;,
  0.693512;0.484851;-0.532879;,
  -0.693560;-0.484997;0.532684;,
  -0.693591;-0.484985;0.532654;,
  -0.693519;-0.484872;0.532852;,
  -0.693532;-0.484782;0.532917;,
  -0.693552;-0.484814;0.532862;,
  -0.693486;-0.484899;0.532870;,
  -0.693466;-0.484972;0.532829;,
  -0.693476;-0.485001;0.532790;,
  -0.693557;-0.484968;0.532714;,
  0.222004;-0.757573;-0.613837;;
  144;
  4;1,10,11,2;,
  4;2,11,12,3;,
  4;3,12,13,4;,
  4;4,13,14,5;,
  4;5,14,15,6;,
  4;6,15,16,7;,
  4;7,16,17,8;,
  4;8,17,18,9;,
  4;9,18,10,1;,
  4;10,19,20,11;,
  4;11,20,21,12;,
  4;12,21,22,13;,
  4;13,22,23,14;,
  4;14,23,24,15;,
  4;15,24,25,16;,
  4;16,25,26,17;,
  4;17,26,27,18;,
  4;18,27,19,10;,
  3;0,127,128;,
  3;0,128,129;,
  3;0,129,130;,
  3;0,130,131;,
  3;0,131,132;,
  3;0,132,133;,
  3;0,133,134;,
  3;0,134,135;,
  3;0,135,127;,
  3;28,136,137;,
  3;28,138,136;,
  3;28,139,138;,
  3;28,140,139;,
  3;28,141,140;,
  3;28,142,141;,
  3;28,143,142;,
  3;28,144,143;,
  3;28,137,144;,
  3;29,30,31;,
  3;29,31,32;,
  3;29,32,33;,
  3;29,33,34;,
  3;29,34,35;,
  3;29,35,36;,
  3;29,36,37;,
  3;29,37,38;,
  3;29,38,39;,
  3;29,39,40;,
  3;29,40,41;,
  3;29,41,30;,
  4;30,42,43,31;,
  4;31,43,44,32;,
  4;32,44,45,33;,
  4;33,45,46,34;,
  4;34,46,47,35;,
  4;35,47,48,36;,
  4;36,48,49,37;,
  4;37,49,50,38;,
  4;38,50,51,39;,
  4;39,51,52,40;,
  4;40,52,53,41;,
  4;41,53,42,30;,
  4;42,54,55,43;,
  4;43,55,56,44;,
  4;44,56,57,45;,
  4;45,57,58,46;,
  4;46,58,59,47;,
  4;47,59,60,48;,
  4;48,60,61,49;,
  4;49,61,62,50;,
  4;50,62,63,51;,
  4;51,63,64,52;,
  4;52,64,65,53;,
  4;53,65,54,42;,
  4;54,66,67,55;,
  4;55,67,68,56;,
  4;56,68,69,57;,
  4;57,69,70,58;,
  4;58,70,71,59;,
  4;59,71,72,60;,
  4;60,72,73,61;,
  4;61,73,74,62;,
  4;62,74,75,63;,
  4;63,75,76,64;,
  4;64,76,77,65;,
  4;65,77,66,54;,
  4;145,78,79,67;,
  4;67,79,80,68;,
  4;68,80,81,69;,
  4;69,81,82,70;,
  4;70,82,83,71;,
  4;71,83,84,72;,
  4;72,84,85,73;,
  4;73,85,86,74;,
  4;74,86,87,75;,
  4;75,87,88,76;,
  4;76,88,89,77;,
  4;77,89,78,66;,
  4;78,90,91,79;,
  4;79,91,92,80;,
  4;80,92,93,81;,
  4;81,93,94,82;,
  4;82,94,95,83;,
  4;83,95,96,84;,
  4;84,96,97,85;,
  4;85,97,98,86;,
  4;86,98,99,87;,
  4;87,99,100,88;,
  4;88,100,101,89;,
  4;89,101,90,78;,
  4;90,102,103,91;,
  4;91,103,104,92;,
  4;92,104,105,93;,
  4;93,105,106,94;,
  4;94,106,107,95;,
  4;95,107,108,96;,
  4;96,108,109,97;,
  4;97,109,110,98;,
  4;98,110,111,99;,
  4;99,111,112,100;,
  4;100,112,113,101;,
  4;101,113,102,90;,
  4;102,114,115,103;,
  4;103,115,116,104;,
  4;104,116,117,105;,
  4;105,117,118,106;,
  4;106,118,119,107;,
  4;107,119,120,108;,
  4;108,120,121,109;,
  4;109,121,122,110;,
  4;110,122,123,111;,
  4;111,123,124,112;,
  4;112,124,125,113;,
  4;113,125,114,102;,
  3;114,126,115;,
  3;115,126,116;,
  3;116,126,117;,
  3;117,126,118;,
  3;118,126,119;,
  3;119,126,120;,
  3;120,126,121;,
  3;121,126,122;,
  3;122,126,123;,
  3;123,126,124;,
  3;124,126,125;,
  3;125,126,114;;
 }
 MeshTextureCoords {
  176;
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.111110;0.500000;,
  0.111110;0.000000;,
  0.222220;0.500000;,
  0.222220;0.000000;,
  0.333330;0.500000;,
  0.333330;0.000000;,
  0.444440;0.500000;,
  0.444440;0.000000;,
  0.555560;0.500000;,
  0.555560;0.000000;,
  0.666670;0.500000;,
  0.666670;0.000000;,
  0.777780;0.500000;,
  0.777780;0.000000;,
  0.888890;0.500000;,
  0.888890;0.000000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.111110;1.000000;,
  0.222220;1.000000;,
  0.333330;1.000000;,
  0.444440;1.000000;,
  0.555560;1.000000;,
  0.666670;1.000000;,
  0.777780;1.000000;,
  0.888890;1.000000;,
  1.000000;1.000000;,
  0.055560;0.000000;,
  0.166670;0.000000;,
  0.277780;0.000000;,
  0.388890;0.000000;,
  0.500000;0.000000;,
  0.611110;0.000000;,
  0.722220;0.000000;,
  0.833330;0.000000;,
  0.944440;0.000000;,
  0.055560;1.000000;,
  0.166670;1.000000;,
  0.277780;1.000000;,
  0.388890;1.000000;,
  0.500000;1.000000;,
  0.611110;1.000000;,
  0.722220;1.000000;,
  0.833330;1.000000;,
  0.944440;1.000000;,
  0.041670;0.000000;,
  0.000000;0.111110;,
  0.083330;0.111110;,
  0.125000;0.000000;,
  0.166670;0.111110;,
  0.208330;0.000000;,
  0.250000;0.111110;,
  0.291670;0.000000;,
  0.333330;0.111110;,
  0.375000;0.000000;,
  0.416670;0.111110;,
  0.458330;0.000000;,
  0.500000;0.111110;,
  0.541670;0.000000;,
  0.583330;0.111110;,
  0.625000;0.000000;,
  0.666670;0.111110;,
  0.708330;0.000000;,
  0.750000;0.111110;,
  0.791670;0.000000;,
  0.833330;0.111110;,
  0.875000;0.000000;,
  0.916670;0.111110;,
  0.958330;0.000000;,
  1.000000;0.111110;,
  0.000000;0.222220;,
  0.083330;0.222220;,
  0.166670;0.222220;,
  0.250000;0.222220;,
  0.333330;0.222220;,
  0.416670;0.222220;,
  0.500000;0.222220;,
  0.583330;0.222220;,
  0.666670;0.222220;,
  0.750000;0.222220;,
  0.833330;0.222220;,
  0.916670;0.222220;,
  1.000000;0.222220;,
  0.000000;0.333330;,
  0.083330;0.333330;,
  0.166670;0.333330;,
  0.250000;0.333330;,
  0.333330;0.333330;,
  0.416670;0.333330;,
  0.500000;0.333330;,
  0.583330;0.333330;,
  0.666670;0.333330;,
  0.750000;0.333330;,
  0.833330;0.333330;,
  0.916670;0.333330;,
  1.000000;0.333330;,
  0.000000;0.444440;,
  0.083330;0.444440;,
  0.166670;0.444440;,
  0.250000;0.444440;,
  0.333330;0.444440;,
  0.416670;0.444440;,
  0.500000;0.444440;,
  0.583330;0.444440;,
  0.666670;0.444440;,
  0.750000;0.444440;,
  0.833330;0.444440;,
  0.916670;0.444440;,
  1.000000;0.444440;,
  0.000000;0.555560;,
  0.083330;0.555560;,
  0.166670;0.555560;,
  0.250000;0.555560;,
  0.333330;0.555560;,
  0.416670;0.555560;,
  0.500000;0.555560;,
  0.583330;0.555560;,
  0.666670;0.555560;,
  0.750000;0.555560;,
  0.833330;0.555560;,
  0.916670;0.555560;,
  1.000000;0.555560;,
  0.000000;0.666670;,
  0.083330;0.666670;,
  0.166670;0.666670;,
  0.250000;0.666670;,
  0.333330;0.666670;,
  0.416670;0.666670;,
  0.500000;0.666670;,
  0.583330;0.666670;,
  0.666670;0.666670;,
  0.750000;0.666670;,
  0.833330;0.666670;,
  0.916670;0.666670;,
  1.000000;0.666670;,
  0.000000;0.777780;,
  0.083330;0.777780;,
  0.166670;0.777780;,
  0.250000;0.777780;,
  0.333330;0.777780;,
  0.416670;0.777780;,
  0.500000;0.777780;,
  0.583330;0.777780;,
  0.666670;0.777780;,
  0.750000;0.777780;,
  0.833330;0.777780;,
  0.916670;0.777780;,
  1.000000;0.777780;,
  0.000000;0.888890;,
  0.083330;0.888890;,
  0.166670;0.888890;,
  0.250000;0.888890;,
  0.333330;0.888890;,
  0.416670;0.888890;,
  0.500000;0.888890;,
  0.583330;0.888890;,
  0.666670;0.888890;,
  0.750000;0.888890;,
  0.833330;0.888890;,
  0.916670;0.888890;,
  1.000000;0.888890;,
  0.041670;1.000000;,
  0.125000;1.000000;,
  0.208330;1.000000;,
  0.291670;1.000000;,
  0.375000;1.000000;,
  0.458330;1.000000;,
  0.541670;1.000000;,
  0.625000;1.000000;,
  0.708330;1.000000;,
  0.791670;1.000000;,
  0.875000;1.000000;,
  0.958330;1.000000;;
 }
}
