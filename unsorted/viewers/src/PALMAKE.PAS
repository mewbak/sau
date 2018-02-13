var
 f:file;
 t:text;
 i,j,k:integer;
 b1,b2,b3:array[0..255,0..2] of byte;
 s,s1:string;
 tmw:array[1..2] of word;
procedure GetPal;
 var
  i,j,k,l:integer;
 begin
  b1:=b3;
  s1:=s;
  i:=Pos(',',s1);
  Delete(s1,1,i);
  byte(s[0]):=Lo(i-1);
  i:=Pos(',',s1);
  Val(Copy(s1,1,i-1),k,j);
  Delete(s1,1,i);
  Val(s1,l,j);
  i:=Pos(':',s);
  Delete(s,1,i);
  Assign(f,s);
  Reset(f,1);
  Blockread(f,tmw,4);
  Seek(f,tmw[2]);
  Blockread(f,b2[k],tmw[1]*3);
  Close(f);
  for i:=k to l do
   b1[i]:=b2[i];
  i:=Pos('.',s);
  byte(s[0]):=Lo(i);
  s:=s+'pal';
  Assign(f,s);
  Rewrite(f,1);
  Blockwrite(f,b1,768);
  Close(f);
 end;
begin
 Assign(t,'info.log');
 Reset(t);
 repeat
  Readln(t,s);
 until s='Fixed palette';
 Readln(t,s);
 GetPal;
 b3:=b1;
 Reset(t);
 repeat
  Readln(t);
  Readln(t,s);
  if Copy(s,1,8)='palette:' then GetPal;
 until Eof(t);
 Close(t);
end.