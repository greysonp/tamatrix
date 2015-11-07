<?php
//header("Content-Type: text/json");

$tama=0;
if (isset($_REQUEST["tama"])) $lastseq=$_REQUEST["tama"];

$shm=shmop_open(7531, "a", 0644, 1024*8);
if ($shm===FALSE) die("Can't open ipc shm key");

$ret=new StdClass();


$off=$tama*(32*48+6)+8;
$data=unpack("c*", shmop_read($shm, $off+4, 32*48));

$ix=array(array(3,2,2,0),array(3,1,3,0));
$ret="";//array();
for ($p=0; $p<128*128/8; $p++) {
	$x=$p%(128/8);
	$y=intval($p/(128/8));
	$b=0xAA;
	$py=intval($y/3)-6;
	$px=($x*4)-8;
	if ($py>=0 && $py<32 && $px<48 && $px>=0) {
		for ($z=0; $z<4; $z++) {
			$b=$b>>2;
			$b|=($ix[$y&1][$data[$px+$z+($py*48)+1]&3])<<6;
		}
	}
	
	$ret.=pack("c",$b);
}

//var_dump($ret);
//echo pack("c*", $ret);
echo $ret;

?>