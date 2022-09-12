// positive sp value has been detected, the output may be wrong!
__int64 __fastcall sub_0(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  void *v4; // rsp
  __int64 v5; // rax
  __int64 v6; // rax
  __int64 v7; // rax
  __int64 v8; // rax
  __int64 v9; // rax
  __int64 v10; // rax
  __int64 v11; // rax
  __int64 v12; // rax
  __int64 v13; // rax
  __int64 v14; // rax
  int v15; // eax
  __int64 v16; // rcx
  int v17; // eax
  unsigned int v18; // eax
  __int64 v19; // rax
  __int64 v20; // rax
  __int64 v21; // rax
  __int64 v22; // rax
  unsigned int v23; // eax
  __int64 v24; // rcx
  __int64 v26; // [rsp-9DC8h] [rbp-9DD8h]
  __int64 v27; // [rsp-9DC8h] [rbp-9DD8h]
  __int64 v28; // [rsp-9DC0h] [rbp-9DD0h]
  __int64 v29; // [rsp-9DB8h] [rbp-9DC8h]
  int v30; // [rsp-9D88h] [rbp-9D98h]
  int v31; // [rsp-9D88h] [rbp-9D98h]
  int v32; // [rsp-9D84h] [rbp-9D94h]
  int v33; // [rsp-9D84h] [rbp-9D94h]
  int v34; // [rsp-9D84h] [rbp-9D94h]
  int v35; // [rsp-9D84h] [rbp-9D94h]
  int v36; // [rsp-9D80h] [rbp-9D90h]
  int v37; // [rsp-9D80h] [rbp-9D90h]
  int v38; // [rsp-9D80h] [rbp-9D90h]
  char v39; // [rsp-9D7Ch] [rbp-9D8Ch] BYREF
  char v40; // [rsp-9D7Bh] [rbp-9D8Bh]
  char v41; // [rsp-9D7Ah] [rbp-9D8Ah]
  __int64 v42; // [rsp-9D78h] [rbp-9D88h]
  char v43; // [rsp-9D70h] [rbp-9D80h]
  char v44; // [rsp-9D6Fh] [rbp-9D7Fh]
  unsigned int kk; // [rsp-9D6Ch] [rbp-9D7Ch]
  unsigned int i7; // [rsp-9D68h] [rbp-9D78h]
  char *v47; // [rsp-9D60h] [rbp-9D70h]
  unsigned int v48; // [rsp-9D58h] [rbp-9D68h] BYREF
  int v49; // [rsp-9D54h] [rbp-9D64h]
  int i4; // [rsp-9D50h] [rbp-9D60h]
  int i9; // [rsp-9D4Ch] [rbp-9D5Ch]
  unsigned int *v52; // [rsp-9D48h] [rbp-9D58h]
  bool v53; // [rsp-9D40h] [rbp-9D50h]
  int v54; // [rsp-9D3Ch] [rbp-9D4Ch]
  unsigned int mm; // [rsp-9D38h] [rbp-9D48h]
  unsigned int *jj; // [rsp-9D30h] [rbp-9D40h]
  int v57; // [rsp-9D28h] [rbp-9D38h]
  int v58; // [rsp-9D24h] [rbp-9D34h]
  int nn; // [rsp-9D20h] [rbp-9D30h]
  unsigned int i6; // [rsp-9D1Ch] [rbp-9D2Ch]
  int i5; // [rsp-9D18h] [rbp-9D28h]
  void (__fastcall *CloseHandle)(__int64); // [rsp-9D10h] [rbp-9D20h]
  char v63[16]; // [rsp-9D08h] [rbp-9D18h] BYREF
  int v64; // [rsp-9CF8h] [rbp-9D08h]
  int i10; // [rsp-9CF4h] [rbp-9D04h]
  unsigned int v66; // [rsp-9CF0h] [rbp-9D00h] BYREF
  unsigned int v67; // [rsp-9CECh] [rbp-9CFCh] BYREF
  int v68; // [rsp-9CE8h] [rbp-9CF8h]
  char *v69; // [rsp-9CE0h] [rbp-9CF0h]
  char v70[16]; // [rsp-9CD8h] [rbp-9CE8h] BYREF
  __int64 v71; // [rsp-9CC8h] [rbp-9CD8h]
  int v72; // [rsp-9CC0h] [rbp-9CD0h]
  int n; // [rsp-9CBCh] [rbp-9CCCh]
  int i8; // [rsp-9CB8h] [rbp-9CC8h]
  int ii; // [rsp-9CB4h] [rbp-9CC4h]
  unsigned int v76; // [rsp-9CB0h] [rbp-9CC0h] BYREF
  int v77; // [rsp-9CACh] [rbp-9CBCh] BYREF
  char *v78; // [rsp-9CA8h] [rbp-9CB8h]
  char *v79; // [rsp-9CA0h] [rbp-9CB0h]
  void (__fastcall *free)(char *); // [rsp-9C98h] [rbp-9CA8h]
  char kernel32[16]; // [rsp-9C90h] [rbp-9CA0h] BYREF
  char v82[8]; // [rsp-9C80h] [rbp-9C90h] BYREF
  _QWORD v83[6]; // [rsp-9C78h] [rbp-9C88h] BYREF
  char v84[8]; // [rsp-9C48h] [rbp-9C58h] BYREF
  char v85[8]; // [rsp-9C40h] [rbp-9C50h] BYREF
  char v86[8]; // [rsp-9C38h] [rbp-9C48h] BYREF
  unsigned int *v87; // [rsp-9C30h] [rbp-9C40h]
  __int64 v88; // [rsp-9C28h] [rbp-9C38h]
  char v89[16]; // [rsp-9C20h] [rbp-9C30h] BYREF
  char v90[16]; // [rsp-9C10h] [rbp-9C20h] BYREF
  char v91[16]; // [rsp-9C00h] [rbp-9C10h] BYREF
  char v92[16]; // [rsp-9BF0h] [rbp-9C00h] BYREF
  char v93[16]; // [rsp-9BE0h] [rbp-9BF0h] BYREF
  char v94[16]; // [rsp-9BD0h] [rbp-9BE0h] BYREF
  char v95[16]; // [rsp-9BC0h] [rbp-9BD0h] BYREF
  char v96[16]; // [rsp-9BB0h] [rbp-9BC0h] BYREF
  char v97[16]; // [rsp-9BA0h] [rbp-9BB0h] BYREF
  char v98[16]; // [rsp-9B90h] [rbp-9BA0h] BYREF
  char v99[16]; // [rsp-9B80h] [rbp-9B90h] BYREF
  char v100[16]; // [rsp-9B70h] [rbp-9B80h] BYREF
  char v101[16]; // [rsp-9B60h] [rbp-9B70h] BYREF
  char v102[16]; // [rsp-9B50h] [rbp-9B60h] BYREF
  char v103[16]; // [rsp-9B40h] [rbp-9B50h] BYREF
  char v104[16]; // [rsp-9B30h] [rbp-9B40h] BYREF
  char v105[16]; // [rsp-9B20h] [rbp-9B30h] BYREF
  char v106[16]; // [rsp-9B10h] [rbp-9B20h] BYREF
  char v107[16]; // [rsp-9B00h] [rbp-9B10h] BYREF
  char v108[16]; // [rsp-9AF0h] [rbp-9B00h] BYREF
  char v109[16]; // [rsp-9AE0h] [rbp-9AF0h] BYREF
  char v110[16]; // [rsp-9AD0h] [rbp-9AE0h] BYREF
  char v111[16]; // [rsp-9AC0h] [rbp-9AD0h] BYREF
  char v112[16]; // [rsp-9AB0h] [rbp-9AC0h] BYREF
  char v113[16]; // [rsp-9AA0h] [rbp-9AB0h] BYREF
  char v114[16]; // [rsp-9A90h] [rbp-9AA0h] BYREF
  char v115[16]; // [rsp-9A80h] [rbp-9A90h] BYREF
  char v116[16]; // [rsp-9A70h] [rbp-9A80h] BYREF
  char v117[24]; // [rsp-9A60h] [rbp-9A70h] BYREF
  char v118[24]; // [rsp-9A48h] [rbp-9A58h] BYREF
  char v119[24]; // [rsp-9A30h] [rbp-9A40h] BYREF
  char v120[24]; // [rsp-9A18h] [rbp-9A28h] BYREF
  char v121[24]; // [rsp-9A00h] [rbp-9A10h] BYREF
  char NtReadVirtualMemory_name[24]; // [rsp-99E8h] [rbp-99F8h] BYREF
  char v123[24]; // [rsp-99D0h] [rbp-99E0h] BYREF
  char v124[24]; // [rsp-99B8h] [rbp-99C8h] BYREF
  char v125[24]; // [rsp-99A0h] [rbp-99B0h] BYREF
  char v126[24]; // [rsp-9988h] [rbp-9998h] BYREF
  char v127[24]; // [rsp-9970h] [rbp-9980h] BYREF
  char v128[24]; // [rsp-9958h] [rbp-9968h] BYREF
  char v129[24]; // [rsp-9940h] [rbp-9950h] BYREF
  char v130[24]; // [rsp-9928h] [rbp-9938h] BYREF
  char v131[24]; // [rsp-9910h] [rbp-9920h] BYREF
  char v132[24]; // [rsp-98F8h] [rbp-9908h] BYREF
  char v133[32]; // [rsp-98E0h] [rbp-98F0h] BYREF
  char v134[32]; // [rsp-98C0h] [rbp-98D0h] BYREF
  char v135[32]; // [rsp-98A0h] [rbp-98B0h] BYREF
  char v136[32]; // [rsp-9880h] [rbp-9890h] BYREF
  char v137[32]; // [rsp-9860h] [rbp-9870h] BYREF
  char v138[32]; // [rsp-9840h] [rbp-9850h] BYREF
  char v139[32]; // [rsp-9820h] [rbp-9830h] BYREF
  char v140[32]; // [rsp-9800h] [rbp-9810h] BYREF
  char v141[32]; // [rsp-97E0h] [rbp-97F0h] BYREF
  char v142[32]; // [rsp-97C0h] [rbp-97D0h] BYREF
  char v143[36]; // [rsp-97A0h] [rbp-97B0h] BYREF
  int v144; // [rsp-977Ch] [rbp-978Ch]
  unsigned __int64 i1; // [rsp-9778h] [rbp-9788h]
  char *v146; // [rsp-9770h] [rbp-9780h]
  __int64 (__fastcall *malloc)(__int64); // [rsp-9768h] [rbp-9778h]
  __int64 v148; // [rsp-9760h] [rbp-9770h]
  unsigned __int64 i2; // [rsp-9758h] [rbp-9768h]
  unsigned __int64 i3; // [rsp-9750h] [rbp-9760h]
  __int64 v151; // [rsp-9748h] [rbp-9758h]
  int v152; // [rsp-9740h] [rbp-9750h]
  int v153; // [rsp-973Ch] [rbp-974Ch]
  int v154; // [rsp-9738h] [rbp-9748h]
  unsigned int v155; // [rsp-9734h] [rbp-9744h] BYREF
  int v156; // [rsp-9730h] [rbp-9740h]
  unsigned int v157; // [rsp-972Ch] [rbp-973Ch] BYREF
  int v158; // [rsp-9728h] [rbp-9738h]
  int v159; // [rsp-9724h] [rbp-9734h]
  int NumOfBytesWritten; // [rsp-9720h] [rbp-9730h]
  int lpNumberOfBytesRead; // [rsp-971Ch] [rbp-972Ch] BYREF
  int v162; // [rsp-9718h] [rbp-9728h]
  __int64 v163; // [rsp-9710h] [rbp-9720h]
  BOOL v164; // [rsp-9708h] [rbp-9718h]
  __int64 v165; // [rsp-9700h] [rbp-9710h]
  __int64 Toolhelp32Snapshot; // [rsp-96F8h] [rbp-9708h]
  __int64 v167; // [rsp-96F0h] [rbp-9700h]
  __int64 v168; // [rsp-96E8h] [rbp-96F8h]
  __int64 v169; // [rsp-96E0h] [rbp-96F0h]
  __int64 v170; // [rsp-96D8h] [rbp-96E8h]
  void (__fastcall *v171)(__int64, unsigned __int64); // [rsp-96D0h] [rbp-96E0h]
  __int64 (__fastcall *v172)(__int64, __int64, _QWORD, unsigned int *); // [rsp-96C8h] [rbp-96D8h]
  __int64 v173; // [rsp-96C0h] [rbp-96D0h]
  _WORD v174[8]; // [rsp-96B8h] [rbp-96C8h] BYREF
  __int64 v175; // [rsp-96A8h] [rbp-96B8h]
  int (__fastcall *v176)(__int64, unsigned __int64, _QWORD, __int64 *, __int64, __int64 *); // [rsp-96A0h] [rbp-96B0h]
  __int64 (__fastcall *v177)(__int64, _QWORD); // [rsp-9698h] [rbp-96A8h]
  __int64 v178; // [rsp-9690h] [rbp-96A0h] BYREF
  unsigned int (*v179)(void); // [rsp-9688h] [rbp-9698h]
  __int64 the_handle; // [rsp-9680h] [rbp-9690h]
  __int64 (__fastcall *v181)(__int64, __int64); // [rsp-9678h] [rbp-9688h]
  __int64 v182; // [rsp-9670h] [rbp-9680h]
  int v183; // [rsp-9668h] [rbp-9678h]
  int v184; // [rsp-9664h] [rbp-9674h] BYREF
  int v185; // [rsp-9660h] [rbp-9670h]
  __int64 v186; // [rsp-9658h] [rbp-9668h]
  __int64 v187; // [rsp-9650h] [rbp-9660h] BYREF
  __int64 v188; // [rsp-9648h] [rbp-9658h]
  _WORD v189[2]; // [rsp-9640h] [rbp-9650h] BYREF
  int v190; // [rsp-963Ch] [rbp-964Ch]
  _BYTE v191[32]; // [rsp-9638h] [rbp-9648h] BYREF
  unsigned __int64 v192; // [rsp-9618h] [rbp-9628h]
  __int64 (__fastcall *CreateFileA)(char *, __int64, __int64, _QWORD, __int64, __int64, _QWORD); // [rsp-9610h] [rbp-9620h]
  __int64 (__fastcall *CreateToolhelp32Snapshot)(__int64, _QWORD); // [rsp-9608h] [rbp-9618h]
  __int64 (__fastcall *v195)(__int64, _QWORD, _QWORD); // [rsp-9600h] [rbp-9610h]
  __int64 v196; // [rsp-95F8h] [rbp-9608h] BYREF
  __int64 v197; // [rsp-95F0h] [rbp-9600h]
  __int64 v198; // [rsp-95E8h] [rbp-95F8h]
  unsigned int (__fastcall *v199)(__int64, _QWORD, _QWORD, __int64 *); // [rsp-95E0h] [rbp-95F0h]
  __int64 (__fastcall *v200)(_BYTE *, __int64, __int64, _QWORD, __int64, __int64, _QWORD); // [rsp-95D8h] [rbp-95E8h]
  __int64 v201; // [rsp-95D0h] [rbp-95E0h] BYREF
  unsigned int (__fastcall *v202)(__int64, __int64, _QWORD, __int64, unsigned int *); // [rsp-95C8h] [rbp-95D8h]
  __int64 (__fastcall *v203)(char *); // [rsp-95C0h] [rbp-95D0h]
  __int64 (__fastcall *v204)(__int64, _QWORD, _QWORD); // [rsp-95B8h] [rbp-95C8h]
  __int64 (__fastcall *v205)(__int64, _QWORD, _BYTE *, _QWORD, char *, __int64, _QWORD, _QWORD); // [rsp-95B0h] [rbp-95C0h]
  _BYTE v206[2]; // [rsp-95A8h] [rbp-95B8h] BYREF
  __int64 v207; // [rsp-95A6h] [rbp-95B6h]
  int v208; // [rsp-959Eh] [rbp-95AEh]
  int v209; // [rsp-959Ah] [rbp-95AAh]
  _BYTE v210[2]; // [rsp-9590h] [rbp-95A0h] BYREF
  __int64 v211; // [rsp-958Eh] [rbp-959Eh]
  int v212; // [rsp-9586h] [rbp-9596h]
  int v213; // [rsp-9582h] [rbp-9592h]
  _BYTE v214[2]; // [rsp-9578h] [rbp-9588h] BYREF
  __int64 v215; // [rsp-9576h] [rbp-9586h]
  int v216; // [rsp-956Eh] [rbp-957Eh]
  int v217; // [rsp-956Ah] [rbp-957Ah]
  __int64 v218; // [rsp-9560h] [rbp-9570h] BYREF
  unsigned __int16 v219; // [rsp-9558h] [rbp-9568h]
  __int16 v220; // [rsp-9556h] [rbp-9566h]
  _BYTE *v221; // [rsp-9550h] [rbp-9560h]
  _BYTE v222[2]; // [rsp-9548h] [rbp-9558h] BYREF
  __int16 v223; // [rsp-9546h] [rbp-9556h]
  __int64 v224; // [rsp-9544h] [rbp-9554h]
  __int64 v225; // [rsp-953Ch] [rbp-954Ch]
  int v226; // [rsp-9534h] [rbp-9544h]
  int v227; // [rsp-9530h] [rbp-9540h]
  _BYTE v228[2]; // [rsp-9528h] [rbp-9538h] BYREF
  __int64 v229; // [rsp-9526h] [rbp-9536h]
  unsigned int v230; // [rsp-951Eh] [rbp-952Eh]
  int v231; // [rsp-951Ah] [rbp-952Ah]
  __int64 v232; // [rsp-9516h] [rbp-9526h]
  __int64 v233; // [rsp-950Eh] [rbp-951Eh]
  __int64 v234; // [rsp-9506h] [rbp-9516h]
  __int64 v235; // [rsp-94F8h] [rbp-9508h] BYREF
  void (__fastcall *v236)(_QWORD, _QWORD, char *, __int64, _BYTE *, int); // [rsp-94F0h] [rbp-9500h]
  __int64 (__fastcall *v237)(__int64, _QWORD); // [rsp-94E8h] [rbp-94F8h]
  unsigned int (__fastcall *v238)(__int64, _BYTE *, __int64, __int64, __int64, __int64 *, int *, __int64 *, __int64 *, __int64 *, _QWORD); // [rsp-94E0h] [rbp-94F0h]
  __int64 (__fastcall *v239)(__int64, __int64, _QWORD, __int64, __int64 *, _QWORD); // [rsp-94D8h] [rbp-94E8h]
  __int64 (__fastcall *v240)(__int64, __int64, _QWORD, _QWORD, char *, int); // [rsp-94D0h] [rbp-94E0h]
  void (__fastcall *v241)(__int64); // [rsp-94C8h] [rbp-94D8h]
  void (__fastcall *v242)(__int64, _QWORD); // [rsp-94C0h] [rbp-94D0h]
  void (__fastcall *v243)(__int64); // [rsp-94B8h] [rbp-94C8h]
  _BYTE *v244; // [rsp-94B0h] [rbp-94C0h]
  void (__fastcall *v245)(_BYTE *, char *, _QWORD); // [rsp-94A8h] [rbp-94B8h]
  unsigned int (__fastcall *v246)(_BYTE *, __int64); // [rsp-94A0h] [rbp-94B0h]
  unsigned int (__fastcall *v247)(__int64, int *); // [rsp-9498h] [rbp-94A8h]
  unsigned int (__fastcall *v248)(__int64, _QWORD, _BYTE *, unsigned int *); // [rsp-9490h] [rbp-94A0h]
  unsigned int (__fastcall *v249)(_BYTE *, _QWORD, _DWORD *); // [rsp-9488h] [rbp-9498h]
  int (__fastcall *v250)(__int64, __int64 *, __int64, _QWORD); // [rsp-9480h] [rbp-9490h]
  __int64 v251; // [rsp-9478h] [rbp-9488h]
  unsigned int (__fastcall *v252)(__int64, int *); // [rsp-9470h] [rbp-9480h]
  __int64 v253; // [rsp-9468h] [rbp-9478h]
  __int64 (__fastcall *v254)(_QWORD); // [rsp-9460h] [rbp-9470h]
  __int64 (__fastcall *v255)(__int64, char *, __int64); // [rsp-9458h] [rbp-9468h]
  __int64 (__fastcall *v256)(__int64, char *, __int64); // [rsp-9450h] [rbp-9460h]
  __int64 (__fastcall *v257)(__int64, int *); // [rsp-9448h] [rbp-9458h]
  int (__fastcall *NtReadVirtualMemory)(_QWORD, __int64, _BYTE *, _QWORD, _QWORD); // [rsp-9440h] [rbp-9450h]
  __int64 v259; // [rsp-9438h] [rbp-9448h]
  __int64 v260; // [rsp-9430h] [rbp-9440h]
  unsigned int (__fastcall *ReadFile)(__int64, _BYTE *, __int64, int *, _QWORD); // [rsp-9428h] [rbp-9438h]
  unsigned int (__fastcall *Module32NextW)(__int64, _DWORD *); // [rsp-9420h] [rbp-9430h]
  __int64 (__fastcall *WideCharToMultiByte)(__int64, _QWORD, __int64 *, __int64, char *, int, _QWORD, _QWORD); // [rsp-9418h] [rbp-9428h]
  unsigned int (__fastcall *Module32FirstW)(__int64, _DWORD *); // [rsp-9410h] [rbp-9420h]
  unsigned int (__fastcall *v265)(__int64, _QWORD, char *, int *); // [rsp-9408h] [rbp-9418h]
  __int64 (__fastcall *v266)(__int64); // [rsp-9400h] [rbp-9410h]
  unsigned int (__fastcall *v267)(__int64, _WORD *, _QWORD); // [rsp-93F8h] [rbp-9408h]
  int (__fastcall *v268)(__int64, __int64, unsigned __int16 *, __int64, _QWORD); // [rsp-93F0h] [rbp-9400h]
  unsigned int (__fastcall *v269)(__int64, _QWORD, __int64, __int64 *, __int64, _DWORD, __int64); // [rsp-93E8h] [rbp-93F8h]
  __int64 (__fastcall *v270)(__int64, __int64); // [rsp-93E0h] [rbp-93F0h]
  void (__fastcall *v271)(__int16 *); // [rsp-93D8h] [rbp-93E8h]
  unsigned int (__fastcall *v272)(_QWORD *, __int64, unsigned int *); // [rsp-93D0h] [rbp-93E0h]
  __int64 v273; // [rsp-93C8h] [rbp-93D8h]
  __int64 (__fastcall *v274)(_QWORD, char *, __int64); // [rsp-93C0h] [rbp-93D0h]
  __int64 (*v275)(void); // [rsp-93B8h] [rbp-93C8h]
  __int64 v276; // [rsp-93B0h] [rbp-93C0h]
  int (__fastcall *v277)(__int64, unsigned __int64, _BYTE *, __int64, _QWORD); // [rsp-93A8h] [rbp-93B8h]
  unsigned int (__fastcall *v278)(unsigned __int64, __int64); // [rsp-93A0h] [rbp-93B0h]
  __int64 v279; // [rsp-9398h] [rbp-93A8h] BYREF
  __int64 v280; // [rsp-9390h] [rbp-93A0h] BYREF
  __int64 (__fastcall *v281)(_BYTE *, __int64); // [rsp-9388h] [rbp-9398h]
  int v282; // [rsp-9380h] [rbp-9390h]
  int v283; // [rsp-937Ch] [rbp-938Ch] BYREF
  __int64 v284; // [rsp-9378h] [rbp-9388h] BYREF
  __int64 v285; // [rsp-9370h] [rbp-9380h] BYREF
  int v286; // [rsp-9358h] [rbp-9368h]
  int v287; // [rsp-9350h] [rbp-9360h]
  int v288; // [rsp-934Ch] [rbp-935Ch]
  int v289; // [rsp-9348h] [rbp-9358h]
  __int64 v290; // [rsp-9340h] [rbp-9350h]
  _DWORD v291[12]; // [rsp-9338h] [rbp-9348h] BYREF
  _BYTE v292[128]; // [rsp-9308h] [rbp-9318h] BYREF
  __int16 v293; // [rsp-9288h] [rbp-9298h] BYREF
  int v294; // [rsp-9258h] [rbp-9268h] BYREF
  unsigned int v295; // [rsp-9250h] [rbp-9260h]
  int v296; // [rsp-922Ch] [rbp-923Ch]
  int v297; // [rsp-9228h] [rbp-9238h]
  int v298; // [rsp-9224h] [rbp-9234h]
  __int64 v299; // [rsp-9128h] [rbp-9138h] BYREF
  _BYTE v300[16]; // [rsp-9120h] [rbp-9130h] BYREF
  _BYTE v301[720]; // [rsp-90F8h] [rbp-9108h] BYREF
  _BYTE v302[1024]; // [rsp-8E28h] [rbp-8E38h] BYREF
  _DWORD moduleentry32[6]; // [rsp-8A28h] [rbp-8A38h] BYREF
  __int64 v304; // [rsp-8A10h] [rbp-8A20h]
  int v305; // [rsp-8A08h] [rbp-8A18h]
  __int64 lpWideCharStr; // [rsp-87F8h] [rbp-8808h] BYREF
  _BYTE v307[512]; // [rsp-85E8h] [rbp-85F8h] BYREF
  unsigned __int16 v308; // [rsp-83E8h] [rbp-83F8h] BYREF
  __int64 v309; // [rsp-83E0h] [rbp-83F0h]
  _BYTE v310[4096]; // [rsp-7FE8h] [rbp-7FF8h] BYREF
  _QWORD v311[1024]; // [rsp-6FE8h] [rbp-6FF8h] BYREF
  char v312; // [rsp-4FE8h] [rbp-4FF8h] BYREF
  char v313; // [rsp-4FE7h] [rbp-4FF7h]
  _BYTE lpBuffer[6]; // [rsp-4FE6h] [rbp-4FF6h] BYREF
  char v315; // [rsp-27F8h] [rbp-2808h] BYREF
  char v316; // [rsp-27F7h] [rbp-2807h]
  _BYTE v317[6]; // [rsp-27F6h] [rbp-2806h] BYREF
  void (__fastcall *v318)(char *, __int64, _QWORD); // [rsp+20h] [rbp+10h]
  __int64 (__fastcall *GetModuleHandle)(char *); // [rsp+28h] [rbp+18h]
  __int64 (__fastcall *GetProcAddress)(__int64, char *); // [rsp+30h] [rbp+20h]

  HIDWORD(v29) = HIDWORD(a4);
  HIDWORD(v28) = HIDWORD(a3);
  HIDWORD(v26) = HIDWORD(a2);
  v4 = alloca(((__int64 (__fastcall *)(__int64, __int64, __int64))sub_4DC9)(a1, a2, a3));
  strcpy(kernel32, "KERNEL32.dll");
  v42 = GetModuleHandle(kernel32);
  v39 = 0;
  v40 = 10;
  v41 = 1;
  v318(&v39, 3i64, 0i64);
  strcpy(v70, "msvcrt.dll");
  strcpy(v84, "malloc");
  v5 = GetModuleHandle(v70);
  malloc = (__int64 (__fastcall *)(__int64))GetProcAddress(v5, v84);
  strcpy(v82, "free");
  v6 = GetModuleHandle(v70);
  free = (void (__fastcall *)(char *))GetProcAddress(v6, v82);
  strcpy(v85, "strncpy");
  v7 = GetModuleHandle(v70);
  v245 = (void (__fastcall *)(_BYTE *, char *, _QWORD))GetProcAddress(v7, v85);
  strcpy(v94, "CloseHandle");
  CloseHandle = (void (__fastcall *)(__int64))GetProcAddress(v42, v94);
  strcpy(v101, "CreateFileA");
  CreateFileA = (__int64 (__fastcall *)(char *, __int64, __int64, _QWORD, __int64, __int64, _QWORD))GetProcAddress(v42, v101);
  v57 = 0;
  v163 = 0i64;
  strcpy(v93, "ntdll.dll");
  v151 = GetModuleHandle(v93);
  strcpy(v63, "USER32.dll");
  strcpy(v133, "CreateToolhelp32Snapshot");
  CreateToolhelp32Snapshot = (__int64 (__fastcall *)(__int64, _QWORD))GetProcAddress(v42, v133);
  v169 = CreateToolhelp32Snapshot(2i64, 0i64);
  if ( v169 != -1 )
  {
    strcpy(v111, "Process32First");
    v247 = (unsigned int (__fastcall *)(__int64, int *))GetProcAddress(v42, v111);
    v294 = 304;
    if ( v247(v169, &v294) )
    {
      v78 = (char *)malloc(20480i64);
      *v78 = 0;
      v78[1] = 11;
      v49 = 2;
      strcpy(v108, "Process32Next");
      v252 = (unsigned int (__fastcall *)(__int64, int *))GetProcAddress(v42, v108);
      do
      {
        strcpy(v100, "OpenProcess");
        v204 = (__int64 (__fastcall *)(__int64, _QWORD, _QWORD))GetProcAddress(v42, v100);
        v168 = v204(4096i64, 0i64, v295);
        strcpy(v138, "QueryFullProcessImageNameW");
        v8 = GetModuleHandle(kernel32);
        v248 = (unsigned int (__fastcall *)(__int64, _QWORD, _BYTE *, unsigned int *))GetProcAddress(v8, v138);
        strcpy(v129, "WideCharToMultiByte");
        v205 = (__int64 (__fastcall *)(__int64, _QWORD, _BYTE *, _QWORD, char *, __int64, _QWORD, _QWORD))GetProcAddress(v42, v129);
        v48 = 256;
        v164 = v168
            && v248(v168, 0i64, v302, &v48)
            && (LODWORD(v28) = 255, (v48 = v205(65001i64, 0i64, v302, v48, &v78[v49 + 5], v28, 0i64, 0i64)) != 0);
        v53 = v164;
        if ( v164 )
        {
          strcpy(v132, "GetFileAttributesExW");
          v249 = (unsigned int (__fastcall *)(_BYTE *, _QWORD, _DWORD *))GetProcAddress(v42, v132);
          if ( v249(v302, 0i64, v291) )
            v152 = v291[8];
          else
            v152 = 0;
          v153 = v152;
        }
        else
        {
          strcpy(v134, "NtQuerySystemInformation");
          v250 = (int (__fastcall *)(__int64, __int64 *, __int64, _QWORD))GetProcAddress(v151, v134);
          v218 = v295;
          v219 = 0;
          v220 = 512;
          v221 = v302;
          if ( v250(88i64, &v218, 24i64, 0i64) < 0 )
          {
            v48 = 0;
          }
          else
          {
                  v251 = (__int64)&v78[v49 + 5];
            LODWORD(v28) = 255;
            v48 = v205(65001i64, 0i64, v221, v219 / 2, (char *)v251, v28, 0i64, 0i64);
          }
          v153 = 0;
        }
        if ( v48 )
        {
          *(_DWORD *)&v78[v49] = v295;
          v78[v49 + 4] = v48;
          *(_DWORD *)&v78[v49 + 5 + v48] = v153;
          v49 += v48 + 9;
        }
        if ( v168 )
          CloseHandle(v168);
        if ( v296 == 1819310181 && v297 == 1919251055 && v298 == 1702389038 )
          v163 = v204(1040i64, 0i64, v295);
      }
      while ( v252(v169, &v294) && v49 < 20219 );
      CloseHandle(v169);
      v318(v78, (unsigned int)v49, 0i64);
      free(v78);
    }
  }
  strcpy(v102, "GetTopWindow");
  v9 = GetModuleHandle(v63);
  v254 = (__int64 (__fastcall *)(_QWORD))GetProcAddress(v9, v102);
  strcpy(v92, "GetWindow");
  v10 = GetModuleHandle(v63);
  v270 = (__int64 (__fastcall *)(__int64, __int64))GetProcAddress(v10, v92);
  strcpy(v135, "GetWindowThreadProcessId");
  v11 = GetModuleHandle(v63);
  v257 = (__int64 (__fastcall *)(__int64, int *))GetProcAddress(v11, v135);
  strcpy(v115, "GetWindowLongA");
  v12 = GetModuleHandle(v63);
  v181 = (__int64 (__fastcall *)(__int64, __int64))GetProcAddress(v12, v115);
  v71 = v254(0i64);
  if ( v71 )
  {
    v69 = (char *)malloc(20480i64);
    *v69 = 0;
    v69[1] = 12;
    v36 = 2;
    do
    {
      if ( (v181(v71, 4294967280i64) & 0x10000000) != 0 )
      {
        strcpy(v114, "GetWindowTextA");
        v13 = GetModuleHandle(v63);
        v255 = (__int64 (__fastcall *)(__int64, char *, __int64))GetProcAddress(v13, v114);
        v144 = v255(v71, &v69[v36 + 1], 64i64);
        for ( n = 0; n < v144; ++n )
          v69[v36 + 1 + n] = 120;
        v69[v36] = v144;
        v37 = v36 + (char)v144 + 1;
        strcpy(v109, "GetClassNameA");
        v14 = GetModuleHandle(v63);
        v256 = (__int64 (__fastcall *)(__int64, char *, __int64))GetProcAddress(v14, v109);
        v154 = v256(v71, &v69[v37 + 1], 64i64);
        v69[v37] = v154;
        v38 = v37 + (char)v154 + 1;
        v185 = v181(v71, 4294967280i64);
        v15 = v181(v71, 4294967276i64);
        *(_DWORD *)&v69[v38] = v15 | v185;
        v282 = v257(v71, &v184);
        *(_DWORD *)&v69[v38 + 4] = v184;
        v36 = v38 + 8;
      }
      v71 = v270(v71, 2i64);
    }
    while ( v71 && v36 <= 20288 );
    v318(v69, (unsigned int)v36, 0i64);
    free(v69);
  }
  strcpy(v104, "LoadLibraryA");
  v203 = (__int64 (__fastcall *)(char *))GetProcAddress(v42, v104);
  strcpy(v91, "psapi.dll");
  v188 = v203(v91);
  strcpy(v128, "GetNativeSystemInfo");
  v271 = (void (__fastcall *)(__int16 *))GetProcAddress(v42, v128);
  v271(&v293);
  if ( v293 == 9 )
  {
    if ( v188 )
    {
      strcpy(v120, "EnumDeviceDrivers");
      v272 = (unsigned int (__fastcall *)(_QWORD *, __int64, unsigned int *))GetProcAddress(v188, v120);
      if ( v272(v311, 0x2000i64, &v155) )
      {
        if ( v155 <= 0x2000ui64 )
        {
          strcpy(v136, "GetDeviceDriverFileNameA");
          v274 = (__int64 (__fastcall *)(_QWORD, char *, __int64))GetProcAddress(v188, v136);
          strcpy(v127, "MultiByteToWideChar");
          v236 = (void (__fastcall *)(_QWORD, _QWORD, char *, __int64, _BYTE *, int))GetProcAddress(v42, v127);
          strcpy(v99, "GetFileSize");
          v237 = (__int64 (__fastcall *)(__int64, _QWORD))GetProcAddress(v42, v99);
          strcpy(v98, "crypt32.dll");
          v88 = v203(v98);
          strcpy(v118, "CryptQueryObject");
          v238 = (unsigned int (__fastcall *)(__int64, _BYTE *, __int64, __int64, __int64, __int64 *, int *, __int64 *, __int64 *, __int64 *, _QWORD))GetProcAddress(v88, v118);
          strcpy(v117, "CryptMsgGetParam");
          v202 = (unsigned int (__fastcall *)(__int64, __int64, _QWORD, __int64, unsigned int *))GetProcAddress(v88, v117);
          strcpy(v139, "CertFindCertificateInStore");
          v239 = (__int64 (__fastcall *)(__int64, __int64, _QWORD, __int64, __int64 *, _QWORD))GetProcAddress(v88, v139);
          strcpy(v121, "CertGetNameStringA");
          v240 = (__int64 (__fastcall *)(__int64, __int64, _QWORD, _QWORD, char *, int))GetProcAddress(v88, v121);
          strcpy(v140, "CertCompareCertificateName");
          v290 = GetProcAddress(v88, v140);
          strcpy(v141, "CertFreeCertificateContext");
          v241 = (void (__fastcall *)(__int64))GetProcAddress(v88, v141);
          strcpy(v113, "CertCloseStore");
          v242 = (void (__fastcall *)(__int64, _QWORD))GetProcAddress(v88, v113);
          strcpy(v110, "CryptMsgClose");
          v243 = (void (__fastcall *)(__int64))GetProcAddress(v88, v110);
          v47 = (char *)malloc(30000i64);
          *v47 = 0;
          v47[1] = 13;
          v30 = 2;
          strcpy(v143, "Wow64EnableWow64FsRedirection");
          v171 = (void (__fastcall *)(__int64, unsigned __int64))GetProcAddress(v42, v143);
          if ( v171 )
            ((void (__fastcall *)(_QWORD))v171)(0i64);
          for ( ii = 0; ; ++ii )
          {
            v273 = ii;
            v16 = ii;
            if ( ii >= v155 / 8ui64 )
              break;
                  v156 = v274(v311[ii], &v47[v30 + 1], 256i64);
            v47[v30] = v156;
            if ( (_BYTE)v156 )
            {
                      v54 = v30;
              LODWORD(v28) = 0;
              LODWORD(v26) = 3;
              v170 = CreateFileA(&v47[v30 + 1], 0x80000000i64, 1i64, 0i64, v26, v28, 0i64);
              if ( v170 != -1 )
              {
                          v236(0i64, 0i64, &v47[v30 + 1], 0xFFFFFFFFi64, v307, 256);
              }
              v31 = v30 + v47[v30] + 1;
              v17 = v237(v170, 0i64);
              *(_DWORD *)&v47[v31] = v17;
              v31 += 4;
              CloseHandle(v170);
              v47[v31] = 0;
              v30 = v31 + 1;
              if ( v170 != -1 )
              {
                LODWORD(v26) = 0;
                if ( v238(1i64, v307, 1024i64, 2i64, v26, &v235, &v283, &v280, &v201, &v187, 0i64) )
                {
                  if ( v202(v187, 6i64, 0i64, 0i64, &v157) )
                  {
                    v165 = malloc(v157);
                    if ( v165 )
                    {
                      if ( v202(v187, 6i64, 0i64, v165, &v157) )
                      {
                        qmemcpy(v301, (const void *)(v165 + 8), 0x10ui64);
                        qmemcpy(v300, (const void *)(v165 + 24), sizeof(v300));
                        v186 = v239(v201, 65537i64, 0i64, 720896i64, &v299, 0i64);
                        if ( v186 )
                        {
                          v158 = v240(v186, 4i64, 0i64, 0i64, &v47[v30], 256);
                          v47[v30 - 1] = v158;
                          if ( (_BYTE)v158 )
                          {
                            v44 = v47[v30 - 1] - 1;
                            v47[v30 - 1] = v44;
                            v30 += v44;
                          }
                          v241(v186);
                        }
                      }
                      free((char *)v165);
                    }
                  }
                  v242(v201, 0i64);
                  v243(v187);
                }
                if ( *(_DWORD *)&v47[v54 - 17 + v47[v54]] == 1819042115
                  && *(_DWORD *)&v47[v54 - 13 + v47[v54]] == 1801675106
                  && (unsigned __int64)v57 < 2 )
                {
                              v18 = v47[v54];
                  v244 = &v302[256 * (__int64)v57 + 512];
                  v245(v244, &v47[v54 + 1], v18);
                  v302[256 * (__int64)v57++ + 512 + v47[v54]] = 0;
                }
              }
            }
          }
          if ( v171 )
          {
            LOBYTE(v16) = 1;
            v171(v16, v155 % 8ui64);
          }
          v318(v47, (unsigned int)v30, 0i64);
          free(v47);
        }
      }
    }
  }
  strcpy(v86, "realloc");
  v19 = GetModuleHandle(v70);
  v177 = (__int64 (__fastcall *)(__int64, _QWORD))GetProcAddress(v19, v86);
  strcpy(v137, "NtQuerySystemInformation");
  v172 = (__int64 (__fastcall *)(__int64, __int64, _QWORD, unsigned int *))GetProcAddress(v151, v137);
  v148 = 0i64;
  v66 = 336;
  while ( 1 )
  {
    v66 += 1024;
    v148 = v177(v148, v66);
    if ( !v148 )
      break;
    v68 = v172(5i64, v148, v66, &v66);
    if ( v68 != -1073741820 )
    {
      if ( v68 >= 0 )
      {
        v76 = 0;
        v172(11i64, (__int64)&v76, 0i64, &v76);
        v87 = (unsigned int *)v177(0i64, v76);
        if ( v87 )
        {
          if ( (int)v172(11i64, (__int64)v87, v76, 0i64) >= 0 )
          {
            for ( jj = (unsigned int *)v148; *((_QWORD *)jj + 10) != 4i64; jj = (unsigned int *)((char *)jj + *jj) )
            {
              if ( !*jj )
                goto LABEL_101;
            }
            v143[32] = 0;
            for ( kk = 0; kk < jj[1]; ++kk )
            {
              for ( mm = 0;
                    mm < *v87
                 && (*(_QWORD *)&jj[20 * kk + 72] < *(_QWORD *)&v87[74 * mm + 6]
                  || *(_QWORD *)&jj[20 * kk + 72] >= *(_QWORD *)&v87[74 * mm + 6] + (unsigned __int64)v87[74 * mm + 8]);
                    ++mm )
              {
                ;
              }
              if ( mm == *v87 )
              {
                v228[0] = 0;
                v228[1] = 15;
                v229 = *(_QWORD *)&jj[20 * kk + 72];
                v230 = jj[20 * kk + 76];
                v231 = jj[1] - (kk + 1);
                v232 = *(_QWORD *)&jj[20 * kk + 68] - *((_QWORD *)jj + 4);
                v233 = 0i64;
                strcpy(v97, "CreateFileA");
                v200 = (__int64 (__fastcall *)(_BYTE *, __int64, __int64, _QWORD, __int64, __int64, _QWORD))GetProcAddress(v42, v97);
                strcpy(v96, "GetFileTime");
                v199 = (unsigned int (__fastcall *)(__int64, _QWORD, _QWORD, __int64 *))GetProcAddress(v42, v96);
                strcpy(v131, "GetWindowsDirectoryA");
                v246 = (unsigned int (__fastcall *)(_BYTE *, __int64))GetProcAddress(v42, v131);
                if ( v246(v292, 128i64) )
                {
                  LODWORD(v28) = 0x2000000;
                  LODWORD(v26) = 3;
                  v182 = v200(v292, 0x80000000i64, 7i64, 0i64, v26, v28, 0i64);
                  if ( v182 != -1 )
                  {
                    if ( v199(v182, 0i64, 0i64, &v284) )
                      v233 = v284 - *(_QWORD *)&jj[20 * kk + 68];
                    CloseHandle(v182);
                  }
                }
                v234 = 0i64;
                strcpy(v126, "GetSystemDirectoryA");
                v281 = (__int64 (__fastcall *)(_BYTE *, __int64))GetProcAddress(v42, v126);
                v72 = v281(v292, 128i64);
                if ( v72 )
                {
                  *(_DWORD *)&v292[v72] = 1769104476;
                  *(_DWORD *)&v292[v72 + 4] = 1936876918;
                  v292[v72 + 8] = 0;
                  LODWORD(v28) = 0x2000000;
                  LODWORD(v26) = 3;
                  v175 = v200(v292, 0x80000000i64, 7i64, 0i64, v26, v28, 0i64);
                  if ( v175 != -1 )
                  {
                    if ( v199(v175, 0i64, 0i64, &v279) )
                      v234 = v279 - *(_QWORD *)&jj[20 * kk + 68];
                    CloseHandle(v175);
                  }
                }
                v318(v228, 42i64, 0i64);
              }
            }
          }
LABEL_101:
          free((char *)v87);
        }
        free((char *)v148);
      }
      break;
    }
  }
  v58 = 0;
  strcpy(v130, "NtQueryVirtualMemory");
  v176 = (int (__fastcall *)(__int64, unsigned __int64, _QWORD, __int64 *, __int64, __int64 *))GetProcAddress(v151, v130);
  strcpy(v105, "IsBadReadPtr");
  v20 = GetModuleHandle(kernel32);
  v278 = (unsigned int (__fastcall *)(unsigned __int64, __int64))GetProcAddress(v20, v105);
  for ( nn = 0; ; ++nn )
  {
    v159 = v163 ? 2 : 1;
    if ( nn >= v159 )
      break;
    if ( nn )
      v198 = v163;
    else
      v198 = -1i64;
    v197 = v198;
    for ( i1 = 0i64; v176(v197, i1, 0i64, v83, 48i64, &v196) >= 0; i1 = v83[3] + v83[0] )
    {
      if ( LODWORD(v83[4]) == 4096
        && (HIDWORD(v83[4]) == 16 || HIDWORD(v83[4]) == 32 || HIDWORD(v83[4]) == 64)
        && (LODWORD(v83[5]) == 0x20000 || LODWORD(v83[5]) == 0x40000)
        && (v83[0] > (unsigned __int64)sub_0 || v83[3] + v83[0] <= (unsigned __int64)sub_0) )
      {
        v210[0] = 0;
        v210[1] = 16;
        v211 = v83[0];
        v212 = v83[3];
        v213 = LODWORD(v83[5]) | HIDWORD(v83[4]) | LODWORD(v83[4]);
        v318(v210, 18i64, 0i64);
        if ( !nn
          && (v83[3] != 73728i64 && v83[3] >= 0x11000ui64 && v83[3] <= 0x500000ui64
           || v83[3] == 36864i64
           || v83[3] == 28672i64
           || v83[3] >= 0x2000ui64 && v83[3] <= 0xF000ui64 && HIDWORD(v83[4]) == 32)
          && (unsigned __int64)v58 < 0xA )
        {
          *(_QWORD *)&v301[56 * v58 + 160] = v197;
          qmemcpy(&v301[56 * v58++ + 168], v83, 0x30ui64);
        }
      }
      if ( !nn
        && (LODWORD(v83[4]) == 4096 && (HIDWORD(v83[4]) == 1 || !HIDWORD(v83[4]))
         || LODWORD(v83[4]) == 0x10000
         || LODWORD(v83[4]) == 0x2000) )
      {
        v43 = 0;
        for ( i2 = i1; i2 < v83[3] + v83[0] && i2 < v83[0] + 0x40000000i64; i2 += 0x20000i64 )
        {
              if ( !v278(i2, 1i64) )
          {
                  if ( v176(-1i64, i2, 0i64, &v285, 48i64, &v196) >= 0
              && v287 == LODWORD(v83[4])
              && (v287 != 4096 || v288 == HIDWORD(v83[4])) )
            {
              if ( !v43 )
              {
                v214[0] = 0;
                v214[1] = 16;
                v215 = v83[0];
                v216 = v83[3];
                v217 = LODWORD(v83[5]) | HIDWORD(v83[4]) | LODWORD(v83[4]);
                v318(v214, 18i64, 0i64);
                v43 = 1;
              }
              v206[0] = 0;
              v206[1] = 16;
              v207 = v285;
              v208 = v286;
              v209 = v289 | v288 | v287;
              v318(v206, 18i64, 0i64);
            }
          }
        }
      }
    }
  }
  strcpy(v125, "NtReadVirtualMemory");
  v277 = (int (__fastcall *)(__int64, unsigned __int64, _BYTE *, __int64, _QWORD))GetProcAddress(v151, v125);
  v189[0] = 1372;
  v190 = 9;
  v191[0] = -104;
  v191[1] = 37;
  v191[2] = 0x80;
  v191[3] = 0;
  v191[4] = 0;
  v191[5] = 0;
  v191[6] = -123;
  v191[7] = -64;
  v191[8] = 117;
  memset(&v191[9], 0, 0x17ui64);
  for ( i1 = 0i64; v176(-1i64, i1, 0i64, v83, 48i64, &v196) >= 0; i1 = v83[3] + v83[0] )
  {
    if ( LODWORD(v83[4]) == 4096
      && (HIDWORD(v83[4]) == 16 || HIDWORD(v83[4]) == 32 || HIDWORD(v83[4]) == 64 || HIDWORD(v83[4]) == 128) )
    {
      for ( i3 = i1; i3 != v83[3] + v83[0]; i3 += 4096i64 )
      {
        if ( v277(-1i64, i3, v310, 4096i64, 0i64) >= 0 )
        {
          for ( i4 = 0; !i4; ++i4 )
          {
            for ( i5 = 0; (unsigned int)(*(_DWORD *)&v191[40 * i4 - 4] + i5) <= 0x1000ui64; ++i5 )
            {
              if ( (_BYTE *)(i5 + i3) != &v191[40 * i4] )
              {
                for ( i6 = 0;
                      i6 < *(_DWORD *)&v191[40 * i4 - 4] && v310[i6 + i5] == *((_BYTE *)&v189[20 * i4 + 4] + (int)i6);
                      ++i6 )
                {
                  ;
                }
                if ( i6 == *(_DWORD *)&v191[40 * i4 - 4] )
                {
                  v222[0] = 0;
                  v222[1] = 53;
                  v223 = v189[20 * i4];
                  v224 = i5 + i3;
                  v225 = v83[0];
                  v226 = v83[3];
                  v227 = LODWORD(v83[5]) | HIDWORD(v83[4]) | LODWORD(v83[4]);
                  v318(v222, 28i64, 0i64);
                }
              }
            }
          }
        }
      }
    }
  }
  v146 = (char *)malloc(10240i64);
  *v146 = 0;
  v146[1] = 17;
  v32 = 2;
  strcpy(v124, "GetCurrentProcessId");
  v179 = (unsigned int (*)(void))GetProcAddress(v42, v124);
  strcpy(v95, "OpenProcess");
  v195 = (__int64 (__fastcall *)(__int64, _QWORD, _QWORD))GetProcAddress(v42, v95);
  strcpy(v116, "DuplicateHandle");
  v269 = (unsigned int (__fastcall *)(__int64, _QWORD, __int64, __int64 *, __int64, _DWORD, __int64))GetProcAddress(v42, v116);
  strcpy(v119, "GetCurrentProcess");
  v275 = (__int64 (*)(void))GetProcAddress(v42, v119);
  strcpy(v106, "NtQueryObject");
  v268 = (int (__fastcall *)(__int64, __int64, unsigned __int16 *, __int64, _QWORD))GetProcAddress(v151, v106);
  strcpy(v90, "_wcsnicmp");
  v21 = GetModuleHandle(v70);
  v267 = (unsigned int (__fastcall *)(__int64, _WORD *, _QWORD))GetProcAddress(v21, v90);
  strcpy(v103, "GetProcessId");
  v266 = (__int64 (__fastcall *)(__int64))GetProcAddress(v42, v103);
  strcpy(v142, "QueryFullProcessImageNameA");
  v265 = (unsigned int (__fastcall *)(__int64, _QWORD, char *, int *))GetProcAddress(v42, v142);
  v52 = 0i64;
  v67 = 32;
  do
  {
    v67 += 1024;
    v52 = (unsigned int *)v177((__int64)v52, v67);
    if ( !v52 )
      break;
    v68 = v172(16i64, (__int64)v52, v67, &v67);
  }
  while ( v68 == -1073741820 );
  if ( v52 && v68 >= 0 )
  {
    v64 = -1;
    for ( i7 = 0; i7 < *v52 && v32 <= 10107; ++i7 )
    {
      if ( v64 == -1 || LOBYTE(v52[6 * i7 + 3]) == v64 )
      {
        v276 = 24i64 * (int)i7;
        if ( v52[(unsigned __int64)v276 / 4 + 2] != v179() )
        {
          v167 = v195(64i64, 0i64, v52[6 * i7 + 2]);
          if ( v167 )
          {
            v22 = v275();
            LODWORD(v29) = 0;
            LODWORD(v27) = 4096;
            if ( v269(v167, HIWORD(v52[6 * i7 + 3]), v22, &v178, v27, 0, v29) )
            {
              if ( v64 == -1 )
              {
                v174[0] = 80;
                v174[1] = 114;
                v174[2] = 111;
                v174[3] = 99;
                v174[4] = 101;
                v174[5] = 115;
                v174[6] = 115;
                v174[7] = 0;
                if ( v268(v178, 2i64, &v308, 1024i64, 0i64) >= 0 && !v267(v309, v174, v308 / 2) )
                  v64 = LOBYTE(v52[6 * i7 + 3]);
              }
              if ( v64 != -1 )
              {
                v183 = v266(v178);
                if ( v183 == v179() && (v52[6 * i7 + 6] & 0x30) != 0 )
                {
                  v173 = v195(4096i64, 0i64, v52[6 * i7 + 2]);
                  v77 = 128;
                  if ( !v173 || !v265(v173, 0i64, &v146[v32 + 1], &v77) )
                    v77 = 0;
                  if ( v173 )
                    CloseHandle(v173);
                  v146[v32] = v77;
                  v33 = v32 + (char)v77 + 1;
                  *(_DWORD *)&v146[v33] = v52[6 * i7 + 6];
                  v32 = v33 + 4;
                }
              }
              CloseHandle(v178);
              CloseHandle(v167);
            }
            else
            {
              CloseHandle(v167);
            }
          }
        }
      }
    }
  }
  if ( v52 )
    free((char *)v52);
  v318(v146, (unsigned int)v32, 0i64);
  free(v146);
  v23 = v179();
  Toolhelp32Snapshot = CreateToolhelp32Snapshot(8i64, v23);
  if ( Toolhelp32Snapshot != -1 )
  {
    v79 = (char *)malloc(20480i64);
    *v79 = 0;
    v79[1] = 18;
    v34 = 2;
    moduleentry32[0] = 1080;
    if ( Module32FirstW(Toolhelp32Snapshot, moduleentry32) )
    {
      do
      {
        NumOfBytesWritten = WideCharToMultiByte(65001i64, 0i64, &lpWideCharStr, 0xFFFFFFFFi64, &v79[v34 + 1], 255, 0i64, 0i64);
        v79[v34] = NumOfBytesWritten;
        if ( (_BYTE)NumOfBytesWritten )
        {
          v35 = v34 + (unsigned __int8)v79[v34] + 1;
          *(_QWORD *)&v79[v35] = v304;
          *(_DWORD *)&v79[v35 + 8] = v305;
          v34 = v35 + 12;
        }
      }
      while ( Module32NextW(Toolhelp32Snapshot, moduleentry32) && v34 <= 20212 );
    }
    CloseHandle(Toolhelp32Snapshot);
    v318(v79, (unsigned int)v34, 0i64);
    free(v79);
  }
  v39 = 0;
  v40 = 10;
  v41 = 2;
  v318(&v39, 3i64, 0i64);
  for ( i8 = 0; i8 < v57; ++i8 )
  {
    LODWORD(v27) = 3;
    the_handle = CreateFileA(&v302[256 * (__int64)i8 + 512], 0x80000000i64, 1i64, 0i64, v27, 128i64, 0i64);
    if ( the_handle != -1 )
    {
      v312 = 0;
      v313 = -66;
      v318(&v312, 2i64, 0i64);
      while ( ReadFile(the_handle, lpBuffer, 10218i64, &lpNumberOfBytesRead, 0i64) && lpNumberOfBytesRead )
      {
        v312 = 0;
        v313 = -66;
        v318(&v312, (unsigned int)(lpNumberOfBytesRead + 2), 0i64);
      }
      CloseHandle(the_handle);
    }
  }
  for ( i9 = 0; i9 < v58; ++i9 )
  {
    _mm_lfence();
    v315 = 0;
    v316 = -66;
    v318(&v315, 2i64, 0i64);
    for ( i10 = 0; ; ++i10 )
    {
      v260 = i10;
      if ( i10 >= *(_QWORD *)&v301[56 * i9 + 192] / 0x27EAui64 + 1 )
        break;
      v253 = i10;
      v192 = i10 >= *(_QWORD *)&v301[56 * i9 + 192] / 0x27EAui64 ? *(_QWORD *)&v301[56 * i9 + 192] % 0x27EAui64 : 10218i64;
      v162 = v192;
      v24 = *(_QWORD *)&v301[56 * i9 + 168] + 10218i64 * i10;
      v259 = 56i64 * i9;
      if ( NtReadVirtualMemory(*(_QWORD *)&v301[v259 + 160], v24, v317, (unsigned int)v192, 0i64) < 0 )
        break;
      v315 = 0;
      v316 = -66;
      v318(&v315, (unsigned int)(v162 + 2), 0i64);
    }
  }
  if ( v163 )
  {
    _mm_lfence();
    CloseHandle(v163);
  }
  v39 = 0;
  v40 = 10;
  v41 = 3;
  return ((__int64 (__fastcall *)(char *, __int64, _QWORD))v318)(&v39, 3i64, 0i64);
}