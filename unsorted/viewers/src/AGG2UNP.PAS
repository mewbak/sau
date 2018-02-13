uses
 Platform,HexOut;
type
 h0t=record
  sig,ofs,sze:longint;
  end;
 hrt=record
  ofs,sze:longint;
  name:string[12];
  end;
 bf=array[0..$7FFF] of byte;
 h0s=array[1..1500] of h0t;
 hrs=array[1..1500] of hrt;
const
 flg:byte=0;
var
 f,g:file;
 i,j,k,cnt:integer;
 hsz:longint;
 buf:bf;
 hd0:h0s absolute buf;
 hst:array[1..1500,1..15] of char absolute buf;
 hdr:hrs;
 sn:string[12];
 ts:string[15];
begin
 i:=2;
 sn:=ParamStr(1);
 if sn='' then Exit;
 if sn='l' then flg:=0
 else if sn='e' then flg:=1
 else i:=1;
 Assign(f,ParamStr(i));
 sn:=ParamStr(i+1);
 if sn='' then sn:='*';
 Reset(f,1);
 Blockread(f,cnt,2);
 hsz:=longint(cnt)*12;
 Blockread(f,buf,hsz);
 for i:=1 to cnt do
  begin
  hdr[i].ofs:=hd0[i].ofs;
  hdr[i].sze:=hd0[i].sze;
  end;
 Seek(f,hdr[cnt].ofs+hdr[cnt].sze);
 hsz:=longint(cnt)*15;
 Blockread(f,buf,hsz,i);
 if i<>hsz then begin
  Writeln('Error reading filelist');
  Close(f);
  Halt(1);
  end;
 for i:=1 to cnt do
  begin
  ts:=hst[i];
  j:=Pos(#0,ts);
  ts[0]:=char(Lo(j));
  hdr[i].name:=ts;
  end;
 if flg=0 then begin
  for i:=1 to cnt do
   with hdr[i] do
    if Match(name,sn) then
     Writeln('File ',name,' at offset ',DStr(ofs),', size ',sze:6,' bytes');
  end
 else begin
  for i:=1 to cnt do
   with hdr[i] do
    if Match(name,sn) then begin
     Write('Extracting ',name);
     Seek(f,ofs);
     Assign(g,name);
     Rewrite(g,1);
     j:=sze div $8000;
     for k:=1 to j do
      begin
      Blockread(f,buf,$8000);
      Blockwrite(g,buf,$8000);
      end;
     j:=sze mod $8000;
     Blockread(f,buf,j);
     Blockwrite(g,buf,j);
     Close(g);
     Writeln(' - done!');
     end;
  end;
 Close(f);
end.