uses
 Platform,HexOut;
type
 nme=array[1..$32] of char;
 nar=array[0..399] of nme;
 nsr=array[0..399] of string[$31];
const
 flg:byte=0;
var
 f,g:file;
 i,j,k:integer;
 cnt:longint;
 pss:array[0..399] of longint;
 buf:array[0..$7FFF] of byte;
 wp:^nar;
 np:^nsr absolute wp;
 sn:string[$32];
 s,ss,s1,s2,s3:string;
{$F+}
function Scan(c:char;s:string):byte;
 begin
  inline(
  $C4/$7E/$06/         {     les  di,[bp]+6  }
  $26/$8A/$0D/         {     mov  cl,es:[di] }
  $30/$ED/             {     xor  ch,ch      }
  $E3/$0C/             {     jcxz exit       }
  $01/$CF/             {     add  di,cx      }
  $FD/                 {     std             }
  $8A/$46/$0A/         {     mov  al,[bp]+10 }
  $F2/$AE/             {     repne scasb     }
  $FC/                 {     cld             }
  $75/$01/             {     jnz  exit       }
  $41/                 {     inc  cx         }
  $91/                 {exit xchg ax,cx      }
  $8B/$E5/             {     mov  sp,bp      }
  $5D/                 {     pop  bp         }
  $CA/$06/$00);        {     retf 6          }
 end;
{$F-}
begin
 GetDir(0,ss);
 i:=3;
 sn:=ParamStr(1);
 if sn='' then Exit;
 if sn='l' then flg:=0
 else if sn='e' then flg:=1
 else i:=2;
 Assign(f,ParamStr(i-1));
 sn:=ParamStr(i);
 if sn='' then sn:='*';
 Reset(f,1);
 Blockread(f,cnt,4);
 New(wp);
 Blockread(f,wp^,cnt*$32);
 Blockread(f,pss,cnt*4);
 pss[cnt]:=FileSize(f);
 for i:=0 to cnt-1 do
  np^[i]:=Copy(wp^[i],1,Pos(#0,wp^[i])-1);
 if flg=0 then begin
  for i:=0 to cnt-1 do
   if Match(np^[i],sn) then
    Writeln('File ',np^[i],' at offset ',DStr(pss[i]),', size ',
    pss[i+1]-pss[i],' bytes');
  end
 else begin
  for i:=0 to cnt-1 do
   if Match(np^[i],sn) then begin
    s1:=np^[i];
    Write('Extracting ',s1);
    j:=Scan('\',s1);
    s:=Copy(s1,1,j-1);
    Delete(s1,1,j);
    {$I-}
    ChDir(ss+s);
    if IOResult<>0 then begin
     s2:=s;
     s3:='';
     repeat
      j:=Pos('\',s2);
      if j=0 then j:=byte(s2[0])+1;
      s3:=s3+'\'+Copy(s2,1,j-1);
      Delete(s2,1,j);
      ChDir(ss+s3);
      if IOResult<>0 then begin
       MkDir(ss+s3);
       ChDir(ss+s3);
       end;
     until s2='';
     end;
    {$I+}
    Assign(g,s1);
    Rewrite(g,1);
    Seek(f,pss[i]);
    j:=(pss[i+1]-pss[i]) div $8000;
    for k:=1 to j do
     begin
     Blockread(f,buf,$8000);
     Blockwrite(g,buf,$8000);
     end;
    j:=(pss[i+1]-pss[i]) mod $8000;
    Blockread(f,buf,j);
    Blockwrite(g,buf,j);
    Close(g);
    Writeln(' - done!');
    end;
  end;
 Close(f);
end.