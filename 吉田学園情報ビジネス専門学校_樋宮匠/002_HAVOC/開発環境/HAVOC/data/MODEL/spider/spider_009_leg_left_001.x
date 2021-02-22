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
 168;
 0.00000;8.00000;7.25000;,
 -0.00540;0.08420;9.79580;,
 21.88260;37.92560;10.03280;,
 21.91680;46.17080;7.42940;,
 46.40760;76.50320;10.03280;,
 46.45800;84.84620;7.42940;,
 21.70260;28.83320;7.42940;,
 46.12440;67.10360;7.42940;,
 -0.02760;-8.29000;7.25120;,
 -0.05820;-11.48080;-0.38380;,
 21.44760;24.97880;-0.38020;,
 45.71820;62.96000;-0.38020;,
 21.26460;26.75420;-8.19040;,
 45.42300;64.49900;-8.19040;,
 -0.07920;-9.34660;-8.01940;,
 -0.07440;-1.32760;-10.56400;,
 21.29880;35.13800;-10.79320;,
 45.47340;73.01600;-10.79320;,
 21.47880;44.07080;-8.19040;,
 45.75660;82.22240;-8.19040;,
 -0.05160;6.98000;-8.01820;,
 -0.02160;10.26380;-0.38380;,
 21.73320;48.08240;-0.38020;,
 46.16280;86.58260;-0.38020;,
 21.91680;46.17080;7.42940;,
 46.45800;84.84620;7.42940;,
 0.00000;8.00000;7.25000;,
 176.70300;51.13280;-7.99540;,
 176.69521;57.42140;-10.53280;,
 194.70360;35.99420;-9.75100;,
 194.18460;30.43340;-7.41160;,
 206.68739;20.92040;-8.70760;,
 206.52180;16.05740;-6.62980;,
 195.08940;41.85440;-7.40260;,
 206.62680;26.10620;-6.61660;,
 176.49060;64.11440;-7.99300;,
 176.40060;65.31560;7.23500;,
 195.16260;42.84020;6.65180;,
 195.14639;44.32640;-0.37540;,
 176.33459;67.00580;-0.37900;,
 206.41260;28.34180;-0.37060;,
 206.46600;27.00740;5.87600;,
 176.67000;52.32020;7.23740;,
 176.62379;49.68800;-0.37900;,
 193.85640;29.10140;-0.37540;,
 194.17320;31.42040;6.66020;,
 206.21519;14.88140;-0.37060;,
 206.28300;16.95620;5.88920;,
 176.61780;59.01260;9.77480;,
 194.74860;37.30940;8.99960;,
 206.42039;22.14800;7.96700;,
 278.13779;-54.97900;-2.52460;,
 289.92780;-67.17340;-1.62880;,
 286.89841;-65.78320;-2.38060;,
 276.82440;-56.27560;-3.32560;,
 276.72961;-58.71520;-2.51860;,
 289.11420;-69.36460;-1.62460;,
 294.80399;-73.20400;-0.37060;,
 277.81439;-54.63700;1.80620;,
 289.72620;-66.96880;0.89480;,
 287.38681;-63.86500;-0.36520;,
 277.56601;-53.47600;-0.35860;,
 286.08179;-67.39000;-0.36520;,
 275.62381;-58.63240;-0.35860;,
 276.38821;-58.41700;1.80020;,
 288.90479;-69.18100;0.89120;,
 286.57019;-65.47300;1.65080;,
 276.36780;-55.83280;2.60840;,
 235.19881;-8.35540;-5.08960;,
 249.62340;-22.95940;-4.42300;,
 249.00060;-26.58700;-5.77780;,
 234.85381;-12.52000;-6.66820;,
 248.16479;-29.95900;-4.42120;,
 234.29340;-16.39180;-5.09140;,
 259.75861;-38.32000;-4.95400;,
 258.66721;-41.21560;-3.80260;,
 260.67599;-35.24560;-3.80620;,
 234.72780;-7.49200;4.37240;,
 249.10860;-22.18900;3.71180;,
 249.58380;-21.35560;-0.35560;,
 235.05540;-6.51460;-0.35860;,
 260.83020;-33.81220;-0.35440;,
 260.26559;-34.61380;3.09680;,
 247.63680;-30.79960;-0.35560;,
 233.83560;-17.30140;-0.35860;,
 247.67340;-29.22760;3.71000;,
 233.80800;-15.56020;4.37480;,
 258.11700;-41.94220;-0.35440;,
 258.31021;-40.53280;3.09320;,
 248.33099;-25.57480;5.06660;,
 234.21600;-11.38120;5.95040;,
 259.22159;-37.51360;4.24460;,
 220.48560;8.73440;-5.83360;,
 219.99960;4.05440;-7.66240;,
 219.36420;-0.35200;-5.84200;,
 220.29539;9.63560;5.10500;,
 220.50060;10.84040;-0.36400;,
 218.93941;-1.39540;-0.36400;,
 219.10139;0.53060;5.11400;,
 219.69299;5.20760;6.93380;,
 268.72501;-45.52840;-3.21160;,
 267.85681;-48.03400;-4.16020;,
 266.84579;-50.43220;-3.20620;,
 268.41901;-44.92960;2.50100;,
 268.93021;-44.30200;-0.35560;,
 266.41260;-50.86900;-0.35560;,
 266.52359;-49.87660;2.49500;,
 267.40979;-47.33320;3.44900;,
 101.38020;103.97960;7.42940;,
 114.04920;99.12140;7.42940;,
 114.18600;108.31340;10.03280;,
 101.52240;113.21360;10.03280;,
 114.31020;116.65700;7.42940;,
 101.59860;121.48760;7.42940;,
 138.44820;98.10500;10.03280;,
 138.64020;106.26680;7.42940;,
 138.30901;89.08280;7.42940;,
 101.18280;102.07400;-8.19040;,
 114.00780;97.21580;-8.19040;,
 113.98440;95.35220;-0.38020;,
 101.24520;100.65200;-0.38020;,
 138.33299;85.34360;-0.38020;,
 138.46800;87.12860;-8.19040;,
 101.40060;119.57120;-8.19040;,
 114.26880;114.74060;-8.19040;,
 114.13140;105.76520;-10.79320;,
 101.25900;110.66540;-10.79320;,
 138.65939;95.48780;-10.79320;,
 138.79919;104.29760;-8.19040;,
 101.59860;121.48760;7.42940;,
 114.31020;116.65700;7.42940;,
 114.33240;120.65720;-0.38020;,
 101.53620;127.32320;-0.38020;,
 138.77521;108.24560;-0.38020;,
 138.64020;106.26680;7.42940;,
 160.80479;65.32340;7.42940;,
 160.60680;73.56140;10.03280;,
 160.56900;81.30200;7.42940;,
 161.00639;63.66320;-8.19040;,
 160.97940;62.02340;-0.38020;,
 176.62379;49.68800;-0.37900;,
 176.70300;51.13280;-7.99540;,
 160.73579;79.53140;-8.19040;,
 160.85699;71.27720;-10.79320;,
 176.69521;57.42140;-10.53280;,
 176.49060;64.11440;-7.99300;,
 160.56900;81.30200;7.42940;,
 160.60980;83.18240;-0.38020;,
 176.33459;67.00580;-0.37900;,
 176.40060;65.31560;7.23500;,
 68.01900;90.64700;7.42940;,
 68.25300;99.97940;10.03280;,
 68.28120;108.32720;7.42940;,
 89.98860;108.31340;10.03280;,
 90.02160;116.65700;7.42940;,
 89.84220;99.12140;7.42940;,
 67.40520;88.35320;-8.19040;,
 67.66860;86.66360;-0.38020;,
 89.64240;95.35220;-0.38020;,
 89.50260;97.21580;-8.19040;,
 67.66740;106.00880;-8.19040;,
 67.43400;96.89300;-10.79320;,
 89.53500;105.76520;-10.79320;,
 89.68140;114.74060;-8.19040;,
 68.28120;108.32720;7.42940;,
 68.01780;110.21480;-0.38020;,
 89.88120;120.65720;-0.38020;,
 90.02160;116.65700;7.42940;;
 
 148;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;2,6,7,4;,
 4;1,8,6,2;,
 4;8,9,10,6;,
 4;6,10,11,7;,
 4;10,12,13,11;,
 4;9,14,12,10;,
 4;14,15,16,12;,
 4;12,16,17,13;,
 4;16,18,19,17;,
 4;15,20,18,16;,
 4;20,21,22,18;,
 4;18,22,23,19;,
 4;22,24,25,23;,
 4;21,26,24,22;,
 4;27,28,29,30;,
 4;30,29,31,32;,
 4;29,33,34,31;,
 4;28,35,33,29;,
 4;36,37,38,39;,
 4;39,38,33,35;,
 4;38,40,34,33;,
 4;37,41,40,38;,
 4;42,43,44,45;,
 4;45,44,46,47;,
 4;44,30,32,46;,
 4;43,27,30,44;,
 4;36,48,49,37;,
 4;37,49,50,41;,
 4;49,45,47,50;,
 4;48,42,45,49;,
 4;51,52,53,54;,
 4;55,54,53,56;,
 4;57,56,53,52;,
 4;58,59,60,61;,
 4;51,61,60,52;,
 4;57,52,60,59;,
 4;55,56,62,63;,
 4;64,63,62,65;,
 4;57,65,62,56;,
 4;64,65,66,67;,
 4;58,67,66,59;,
 4;57,59,66,65;,
 4;68,69,70,71;,
 4;71,70,72,73;,
 4;70,74,75,72;,
 4;69,76,74,70;,
 4;77,78,79,80;,
 4;80,79,69,68;,
 4;79,81,76,69;,
 4;78,82,81,79;,
 4;73,72,83,84;,
 4;84,83,85,86;,
 4;83,87,88,85;,
 4;72,75,87,83;,
 4;86,85,89,90;,
 4;90,89,78,77;,
 4;89,91,82,78;,
 4;85,88,91,89;,
 4;34,92,93,31;,
 4;31,93,94,32;,
 4;93,71,73,94;,
 4;92,68,71,93;,
 4;41,95,96,40;,
 4;40,96,92,34;,
 4;96,80,68,92;,
 4;95,77,80,96;,
 4;32,94,97,46;,
 4;46,97,98,47;,
 4;97,84,86,98;,
 4;94,73,84,97;,
 4;47,98,99,50;,
 4;50,99,95,41;,
 4;99,90,77,95;,
 4;98,86,90,99;,
 4;76,100,101,74;,
 4;74,101,102,75;,
 4;101,54,55,102;,
 4;100,51,54,101;,
 4;82,103,104,81;,
 4;81,104,100,76;,
 4;104,61,51,100;,
 4;103,58,61,104;,
 4;75,102,105,87;,
 4;87,105,106,88;,
 4;105,63,64,106;,
 4;102,55,63,105;,
 4;88,106,107,91;,
 4;91,107,103,82;,
 4;107,67,58,103;,
 4;106,64,67,107;,
 4;108,109,110,111;,
 4;111,110,112,113;,
 4;110,114,115,112;,
 4;109,116,114,110;,
 4;117,118,119,120;,
 4;120,119,109,108;,
 4;119,121,116,109;,
 4;118,122,121,119;,
 4;123,124,125,126;,
 4;126,125,118,117;,
 4;125,127,122,118;,
 4;124,128,127,125;,
 4;129,130,131,132;,
 4;132,131,124,123;,
 4;131,133,128,124;,
 4;130,134,133,131;,
 4;116,135,136,114;,
 4;114,136,137,115;,
 4;136,48,36,137;,
 4;135,42,48,136;,
 4;122,138,139,121;,
 4;121,139,135,116;,
 4;139,140,42,135;,
 4;138,141,140,139;,
 4;128,142,143,127;,
 4;127,143,138,122;,
 4;143,144,141,138;,
 4;142,145,144,143;,
 4;134,146,147,133;,
 4;133,147,142,128;,
 4;147,148,145,142;,
 4;146,149,148,147;,
 4;7,150,151,4;,
 4;4,151,152,5;,
 4;151,153,154,152;,
 4;150,155,153,151;,
 4;13,156,157,11;,
 4;11,157,150,7;,
 4;157,158,155,150;,
 4;156,159,158,157;,
 4;19,160,161,17;,
 4;17,161,156,13;,
 4;161,162,159,156;,
 4;160,163,162,161;,
 4;25,164,165,23;,
 4;23,165,160,19;,
 4;165,166,163,160;,
 4;164,167,166,165;,
 4;167,129,132,166;,
 4;166,132,123,163;,
 4;163,123,126,162;,
 4;162,126,117,159;,
 4;159,117,120,158;,
 4;158,120,108,155;,
 4;155,108,111,153;,
 4;153,111,113,154;;
 
 MeshMaterialList {
  2;
  148;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.778400;0.800000;0.125600;1.000000;;
   74.000000;
   0.500000;0.500000;0.500000;;
   0.009730;0.010000;0.001570;;
  }
  Material {
   0.213600;0.153600;0.112800;1.000000;;
   7.000000;
   0.850000;0.850000;0.850000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  173;
  -0.730899;0.417751;0.539696;,
  0.619233;0.552982;0.557460;,
  0.698531;-0.411994;0.585078;,
  -0.539733;-0.555564;0.632485;,
  0.719219;-0.427039;-0.548053;,
  -0.564164;-0.586958;-0.580689;,
  -0.706010;0.406534;-0.579897;,
  0.588159;0.529471;-0.611334;,
  0.189176;-0.072119;-0.979291;,
  -0.448893;-0.574687;0.684273;,
  0.557328;0.498315;-0.664129;,
  -0.452828;-0.584721;-0.673089;,
  0.562019;0.497203;0.661002;,
  -0.535308;-0.546233;0.644264;,
  -0.547809;-0.562817;-0.618985;,
  0.594150;0.518451;-0.614975;,
  0.610307;0.528061;0.590489;,
  -0.602638;-0.518347;0.606749;,
  -0.620383;-0.537251;-0.571390;,
  0.672734;0.487661;-0.556432;,
  0.694193;0.499713;0.518056;,
  0.598017;0.467903;-0.650725;,
  0.603778;0.474115;0.640833;,
  -0.526096;-0.520345;0.672654;,
  -0.537984;-0.531678;-0.654134;,
  -0.271254;0.612967;0.742086;,
  0.254837;-0.617605;0.744057;,
  0.285826;-0.688503;-0.666534;,
  -0.243812;0.547663;-0.800388;,
  0.452486;0.594515;0.664687;,
  -0.404062;-0.572077;0.713766;,
  -0.438171;-0.628196;-0.642943;,
  0.414740;0.542759;-0.730345;,
  -0.641921;0.491852;0.588234;,
  0.607369;-0.466128;0.643294;,
  0.643025;-0.493221;-0.585877;,
  -0.607353;0.463819;-0.644976;,
  0.067968;-0.030012;-0.997236;,
  -0.008548;-0.198622;-0.980039;,
  0.609276;0.443291;-0.657477;,
  0.727352;0.686218;-0.007983;,
  0.603410;0.440317;0.664843;,
  -0.640176;-0.768143;0.011432;,
  -0.516939;-0.712147;0.474995;,
  0.067655;-0.030175;0.997252;,
  0.047174;-0.014099;-0.998787;,
  -0.531779;-0.552949;-0.641450;,
  0.056830;-0.022526;-0.998130;,
  0.576131;0.512459;-0.636757;,
  0.741609;0.670319;-0.026266;,
  0.770084;0.637781;-0.014314;,
  0.586623;0.520899;0.620111;,
  -0.713462;-0.700217;0.025845;,
  -0.519359;-0.540162;0.662187;,
  -0.730801;-0.682176;0.023780;,
  0.055208;-0.006654;0.998453;,
  0.055964;-0.022154;0.998187;,
  -0.600114;-0.539817;-0.590305;,
  0.638005;0.501113;-0.584667;,
  0.656268;0.512564;0.553706;,
  -0.584502;-0.523176;0.620197;,
  -0.497650;-0.559448;-0.662844;,
  0.581632;0.469573;-0.664233;,
  0.589267;0.472741;0.655195;,
  -0.489968;-0.544803;0.680530;,
  -0.005042;0.013326;0.999898;,
  0.271150;0.613143;0.741979;,
  0.010741;0.010490;0.999887;,
  -0.249575;-0.616512;0.746743;,
  0.382356;-0.918707;0.098897;,
  -0.563111;-0.821895;0.085993;,
  -0.367137;-0.907799;-0.202759;,
  0.002725;-0.005944;-0.999979;,
  -0.002379;-0.007923;-0.999966;,
  0.239994;0.545878;-0.802758;,
  -0.415058;0.906061;-0.082345;,
  0.603298;0.793426;-0.080659;,
  0.589668;0.550367;0.591091;,
  -0.510001;-0.552757;0.659058;,
  -0.538842;-0.591665;-0.599651;,
  0.551103;0.521167;-0.651667;,
  -0.429857;0.601862;0.673042;,
  0.395824;-0.566710;0.722608;,
  0.436600;-0.622218;-0.649789;,
  -0.396879;0.544877;-0.738645;,
  -0.715593;0.431019;0.549682;,
  -0.013299;0.010070;0.999861;,
  0.684109;-0.419618;0.596587;,
  -0.473755;0.268430;0.838750;,
  0.790252;-0.609835;0.060022;,
  0.708932;-0.436821;-0.553718;,
  0.860859;-0.507543;0.036351;,
  0.006201;-0.005080;-0.999968;,
  -0.687863;0.416221;-0.594647;,
  0.448355;-0.268640;-0.852532;,
  -0.791325;0.608449;-0.059948;,
  -0.866058;0.498667;-0.035706;,
  -0.617725;-0.547337;-0.564657;,
  -0.358663;-0.339954;-0.869363;,
  0.661453;0.495463;-0.563024;,
  -0.122868;-0.132867;-0.983489;,
  0.740533;0.669971;-0.052447;,
  0.798958;0.600290;-0.036308;,
  0.686475;0.510901;0.517428;,
  -0.596899;-0.524593;0.607053;,
  -0.770699;-0.636340;0.033080;,
  -0.696655;-0.715351;0.054262;,
  0.478328;0.320549;0.817588;,
  0.186047;0.129594;0.973957;,
  -0.451546;0.270554;0.850239;,
  0.851420;-0.522782;0.042233;,
  0.434639;-0.268832;-0.859546;,
  -0.854866;0.517144;-0.042024;,
  -0.361726;-0.342901;-0.866933;,
  0.792608;0.608305;-0.041687;,
  -0.759834;-0.648826;0.040947;,
  0.466911;0.330372;0.820273;,
  0.111208;-0.045634;-0.992749;,
  0.749111;0.662425;0.005030;,
  -0.610316;-0.792130;0.006714;,
  0.108979;-0.044438;0.993050;,
  0.047045;-0.016125;-0.998763;,
  0.731598;0.681468;-0.019137;,
  -0.711224;-0.702588;0.023047;,
  0.051574;-0.011455;0.998603;,
  0.201488;0.107251;-0.973602;,
  0.774022;0.632344;-0.032107;,
  -0.758273;-0.651303;0.028745;,
  -0.091387;-0.124319;0.988025;,
  0.065620;-0.027781;-0.997458;,
  0.760409;0.649264;-0.015286;,
  -0.689790;-0.723655;0.022641;,
  0.063281;-0.028765;0.997581;,
  0.005807;0.013545;0.999891;,
  -0.373500;-0.921868;0.103232;,
  -0.002147;-0.006287;-0.999978;,
  0.413873;0.902294;0.120727;,
  0.026339;0.007406;0.999626;,
  -0.671798;-0.737826;0.065574;,
  0.008816;-0.010211;-0.999909;,
  0.730640;0.679830;-0.063218;,
  -0.008387;0.012282;0.999889;,
  0.564957;-0.820313;0.088938;,
  0.004563;-0.006229;-0.999970;,
  -0.581068;0.809584;-0.083265;,
  -0.011192;0.601822;0.798552;,
  -0.015373;0.848157;0.529522;,
  -0.011122;0.533633;-0.845643;,
  -0.000029;-0.005516;-0.999985;,
  0.016077;-0.978056;-0.207719;,
  0.017127;-0.993923;0.108738;,
  0.012041;-0.641504;0.767025;,
  -0.000117;0.013762;0.999905;,
  0.420537;-0.258216;0.869755;,
  0.436954;-0.259036;0.861378;,
  -0.431930;0.260689;-0.863410;,
  -0.453639;0.259957;-0.852428;,
  0.448542;0.320524;-0.834311;,
  0.464099;0.314539;-0.828056;,
  0.418555;0.323967;-0.848444;,
  -0.349233;-0.327169;0.878064;,
  -0.346655;-0.325382;0.879748;,
  -0.355074;-0.324729;0.876626;,
  0.786147;0.617909;0.012706;,
  -0.589308;-0.794877;-0.144525;,
  -0.551137;-0.834280;0.015009;,
  0.182762;-0.069457;0.980701;,
  -0.363855;-0.333750;-0.869609;,
  0.457876;0.317831;0.830261;,
  -0.114911;-0.288881;-0.950444;,
  0.005699;-0.290353;-0.956903;,
  -0.015844;0.756366;-0.653956;,
  0.316656;0.714669;-0.623680;;
  148;
  4;0,88,109,85;,
  4;85,109,86,33;,
  4;153,87,34,86;,
  4;154,2,87,153;,
  4;2,91,110,87;,
  4;87,110,89,34;,
  4;110,90,35,89;,
  4;91,4,90,110;,
  4;4,94,111,90;,
  4;90,111,92,35;,
  4;155,93,36,92;,
  4;156,6,93,155;,
  4;6,96,112,93;,
  4;93,112,95,36;,
  4;112,85,33,95;,
  4;96,0,85,112;,
  4;5,100,113,97;,
  4;97,113,98,18;,
  4;157,99,19,158;,
  4;159,7,99,157;,
  4;1,103,114,101;,
  4;101,114,99,7;,
  4;114,102,19,99;,
  4;103,20,102,114;,
  4;3,106,115,104;,
  4;104,115,105,17;,
  4;115,97,18,105;,
  4;106,5,97,115;,
  4;1,108,116,103;,
  4;103,116,107,20;,
  4;160,104,17,161;,
  4;162,3,104,160;,
  4;10,39,117,37;,
  4;11,37,117,38;,
  4;8,38,117,39;,
  4;12,41,118,40;,
  4;10,40,118,39;,
  4;163,39,118,41;,
  4;11,164,119,42;,
  4;9,42,119,43;,
  4;165,43,119,164;,
  4;9,43,120,44;,
  4;12,44,120,41;,
  4;166,41,120,166;,
  4;15,48,121,45;,
  4;45,121,46,14;,
  4;121,47,24,46;,
  4;48,21,47,121;,
  4;16,51,122,49;,
  4;49,122,48,15;,
  4;122,50,21,48;,
  4;51,22,50,122;,
  4;14,46,123,52;,
  4;52,123,53,13;,
  4;123,54,23,53;,
  4;46,24,54,123;,
  4;13,53,124,55;,
  4;55,124,51,16;,
  4;124,56,22,51;,
  4;53,23,56,124;,
  4;19,58,125,158;,
  4;98,167,57,18;,
  4;125,45,14,57;,
  4;58,15,45,125;,
  4;20,59,126,102;,
  4;102,126,58,19;,
  4;126,49,15,58;,
  4;59,16,49,126;,
  4;18,57,127,105;,
  4;105,127,60,17;,
  4;127,52,13,60;,
  4;57,14,52,127;,
  4;17,60,128,161;,
  4;107,168,59,20;,
  4;128,55,16,59;,
  4;60,13,55,128;,
  4;21,62,129,47;,
  4;47,129,61,24;,
  4;129,37,11,61;,
  4;62,10,37,129;,
  4;22,63,130,50;,
  4;50,130,62,21;,
  4;130,40,10,62;,
  4;63,12,40,130;,
  4;24,61,131,54;,
  4;54,131,64,23;,
  4;131,42,9,64;,
  4;61,11,42,131;,
  4;23,64,132,56;,
  4;56,132,63,22;,
  4;132,44,12,63;,
  4;64,9,44,132;,
  4;151,68,133,152;,
  4;152,133,66,145;,
  4;133,67,29,66;,
  4;68,30,67,133;,
  4;149,71,134,150;,
  4;150,134,68,151;,
  4;134,70,30,68;,
  4;71,31,70,134;,
  4;147,74,135,148;,
  4;148,135,169,170;,
  4;135,73,31,169;,
  4;74,32,73,135;,
  4;145,66,136,146;,
  4;171,172,74,147;,
  4;136,76,32,74;,
  4;66,29,76,136;,
  4;30,78,137,67;,
  4;67,137,77,29;,
  4;137,108,1,77;,
  4;78,3,108,137;,
  4;31,79,138,70;,
  4;70,138,78,30;,
  4;138,106,3,78;,
  4;79,5,106,138;,
  4;32,80,139,73;,
  4;73,139,79,31;,
  4;139,100,5,79;,
  4;80,7,100,139;,
  4;29,77,140,76;,
  4;76,140,80,32;,
  4;140,101,7,80;,
  4;77,1,101,140;,
  4;34,82,141,86;,
  4;86,141,81,33;,
  4;141,65,25,81;,
  4;82,26,65,141;,
  4;35,83,142,89;,
  4;89,142,82,34;,
  4;142,69,26,82;,
  4;83,27,69,142;,
  4;36,84,143,92;,
  4;92,143,83,35;,
  4;143,72,27,83;,
  4;84,28,72,143;,
  4;33,81,144,95;,
  4;95,144,84,36;,
  4;144,75,28,84;,
  4;81,25,75,144;,
  4;25,145,146,75;,
  4;75,171,147,28;,
  4;28,147,148,72;,
  4;72,148,170,27;,
  4;27,149,150,69;,
  4;69,150,151,26;,
  4;26,151,152,65;,
  4;65,152,145,25;;
 }
 MeshTextureCoords {
  168;
  0.375000;0.000000;,
  0.380890;0.125020;,
  0.409930;0.125000;,
  0.410120;0.000000;,
  0.443050;0.125000;,
  0.445240;0.000000;,
  0.409690;0.250000;,
  0.442670;0.250000;,
  0.375000;0.250000;,
  0.375000;0.375000;,
  0.409340;0.375000;,
  0.442120;0.375000;,
  0.409090;0.500000;,
  0.441720;0.500000;,
  0.375000;0.500000;,
  0.375000;0.625000;,
  0.409140;0.625000;,
  0.441790;0.625000;,
  0.409380;0.750000;,
  0.442170;0.750000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.409730;0.875000;,
  0.442720;0.875000;,
  0.410120;1.000000;,
  0.445240;1.000000;,
  0.375000;1.000000;,
  0.875000;0.250000;,
  0.875000;0.125000;,
  0.862480;0.125100;,
  0.834400;0.209480;,
  0.849960;0.125160;,
  0.825020;0.200160;,
  0.834300;0.040730;,
  0.824860;0.050160;,
  0.875000;0.000000;,
  0.625000;0.000000;,
  0.665590;0.040670;,
  0.749940;0.012560;,
  0.750000;0.000000;,
  0.749880;0.025080;,
  0.674900;0.050040;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.749940;0.237650;,
  0.665490;0.209540;,
  0.749880;0.225240;,
  0.674740;0.200280;,
  0.621090;0.125030;,
  0.637400;0.125100;,
  0.649800;0.125160;,
  0.775740;0.099090;,
  0.764990;0.109890;,
  0.774010;0.124940;,
  0.785360;0.124920;,
  0.775660;0.150760;,
  0.764940;0.140000;,
  0.749880;0.124960;,
  0.723750;0.098840;,
  0.734690;0.109750;,
  0.749810;0.100740;,
  0.749740;0.089300;,
  0.749820;0.149130;,
  0.749740;0.160540;,
  0.723830;0.151000;,
  0.734740;0.140140;,
  0.725620;0.124940;,
  0.714120;0.124920;,
  0.806530;0.068350;,
  0.798530;0.076320;,
  0.814790;0.124980;,
  0.825480;0.125020;,
  0.798510;0.173650;,
  0.806560;0.181700;,
  0.804900;0.124960;,
  0.791080;0.166220;,
  0.791120;0.083710;,
  0.692950;0.068090;,
  0.700880;0.076020;,
  0.749700;0.059900;,
  0.749740;0.049280;,
  0.749690;0.069750;,
  0.708260;0.083400;,
  0.749700;0.190070;,
  0.749740;0.200760;,
  0.700900;0.173950;,
  0.692920;0.181960;,
  0.749690;0.180170;,
  0.708300;0.166520;,
  0.684620;0.124980;,
  0.674000;0.125020;,
  0.694480;0.124960;,
  0.815460;0.059500;,
  0.837410;0.125100;,
  0.815560;0.190710;,
  0.684150;0.059300;,
  0.749810;0.037500;,
  0.749800;0.212710;,
  0.684050;0.190910;,
  0.662200;0.125110;,
  0.783990;0.090830;,
  0.795370;0.124930;,
  0.783920;0.159040;,
  0.715420;0.090530;,
  0.749700;0.079260;,
  0.749700;0.170600;,
  0.715480;0.159340;,
  0.704030;0.124930;,
  0.517290;0.250000;,
  0.534400;0.250000;,
  0.534580;0.125000;,
  0.517480;0.125000;,
  0.534820;0.000000;,
  0.516770;0.000000;,
  0.567350;0.125000;,
  0.569310;0.000000;,
  0.567160;0.250000;,
  0.517020;0.500000;,
  0.534340;0.500000;,
  0.534310;0.375000;,
  0.517100;0.375000;,
  0.567190;0.375000;,
  0.567370;0.500000;,
  0.517310;0.750000;,
  0.534690;0.750000;,
  0.534510;0.625000;,
  0.517120;0.625000;,
  0.567630;0.625000;,
  0.567820;0.750000;,
  0.516770;1.000000;,
  0.534820;1.000000;,
  0.534780;0.875000;,
  0.517500;0.875000;,
  0.567790;0.875000;,
  0.569310;1.000000;,
  0.596850;0.250000;,
  0.596960;0.125000;,
  0.597150;0.000000;,
  0.597030;0.500000;,
  0.596890;0.375000;,
  0.625000;0.375000;,
  0.625000;0.500000;,
  0.597320;0.750000;,
  0.597200;0.625000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.597150;1.000000;,
  0.597270;0.875000;,
  0.625000;0.875000;,
  0.625000;1.000000;,
  0.472230;0.250000;,
  0.472550;0.125000;,
  0.472790;0.000000;,
  0.501900;0.125000;,
  0.500330;0.000000;,
  0.501710;0.250000;,
  0.471410;0.500000;,
  0.471760;0.375000;,
  0.501440;0.375000;,
  0.501250;0.500000;,
  0.471760;0.750000;,
  0.471440;0.625000;,
  0.501290;0.625000;,
  0.501490;0.750000;,
  0.472790;1.000000;,
  0.472230;0.875000;,
  0.501760;0.875000;,
  0.500330;1.000000;;
 }
}
