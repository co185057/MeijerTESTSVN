BEGIN { 
  doit="FALSE" 
  printf("\nTBSLISTTYPE tbsListArray[] =\n{\n");
}

$1 == "TB_CLOSED" { doit="TRUE" }
$1 == "}"  && $2 ~ "TBSTATE" && doit == "TRUE" { doit="FALSE" }

doit=="TRUE" { 
  pos = index($1, ",")
  if (pos > 0)
    tbName = substr($1, 0, pos-1)
  else
    tbName = $1
  tbNameConst = "\""tbName"\""
  printf("%-34s,%-34s,0,0,INT_MAX,INT_MIN,\n", tbNameConst, tbName) 
}

END {
  printf("};\n")
  printf("int tbsListArraySize = sizeof(tbsListArray) / sizeof( tbsListArray[0]);\n")

}
