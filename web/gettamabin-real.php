<?php
//header("Content-Type: text/json");

$tama=0;
if (isset($_REQUEST["tama"])) $tama=$_REQUEST["tama"];
if ($tama==-1) {
	echo file_get_contents("had.bin");
	exit(0);
}

$shm=shmop_open(7531, "a", 0644, 1024*8);
if ($shm===FALSE) die("Can't open ipc shm key");


$off=$tama*(32*48+6)+8;
$data=unpack("c*", shmop_read($shm, $off+4, 32*48));
//var_dump($data);
$ret="";
for ($y=0; $y<32; $y++) {
	for ($x=0; $x<48; $x+=4) {
		if ($y<16) $p=((31-$y)*48)+$x+1; else $p=(($y-16)*48)+$x+1;
		$b=0;
		$b+=($data[$p+0]*64);
		$b+=($data[$p+1]*16);
		$b+=($data[$p+2]*4);
		$b+=$data[$p+3];
		$ret.=pack("c",$b);
	}
}
//var_dump($ret);
//echo pack("c*", $ret);
echo $ret;

?>