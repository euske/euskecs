function offline() {
  var a = document.getElementsByTagName("img"); 
  for (var i=0; i < a.length;i++) {
    var url = a[i].src;
    a[i].src = url.substr(url.lastIndexOf("/")+1);
    console.log(a[i].src);
  }
}
