// counter.js

//  Counter
// 
function Counter(parent, decimal)
{
  this.cols = document.createElement("div");;
  this.cols.style.top = 0;
  this.cols.style.left = 0;
  this.cols.style.position = "absolute";
  this.cols.style.overflow = "hidden";
  this.decimal = decimal;
  this.active = true;
  this.focused = true;
  parent.appendChild(this.decimal);
  parent.appendChild(this.cols);
  var counter = this;
  function focus(e) { counter.focused = true; }
  function blur(e) { counter.focused = false; }
  window.addEventListener("focus", focus);
  window.addEventListener("blur", blur);
};

Counter.prototype.init = function (base, ndigits, dwidth, dheight)
{
  function mkelem(x) {
    var e = document.createElement("div");
    e.className = "digit";
    e.style.position = "absolute";
    e.style.top = 0;
    e.style.left = x+"px";
    e.style.width = dwidth+"px";
    e.style.height = dheight+"px";
    return e;
  }
  this.base = base;
  this.digits0 = new Array();
  this.digits1 = new Array();
  for (var i = 0; i < ndigits; i++) {
    var x = (ndigits-1-i)*dwidth;
    this.digits0.push(mkelem(x));
    this.digits1.push(mkelem(x));
  }
  
  var cols = this.cols;
  cols.style.width = (dwidth*ndigits)+"px";
  cols.style.height = dheight+"px";
  for (var i = 0; i < this.digits0.length; i++) {
    cols.appendChild(this.digits0[i]);
  }
  for (var i = 0; i < this.digits1.length; i++) {
    cols.appendChild(this.digits1[i]);
  }
};

Counter.prototype.clean = function ()
{
  var cols = this.cols;
  while (cols.hasChildNodes()) {
    cols.removeChild(cols.firstChild);
  }
};

Counter.prototype.start = function (fps, dps)
{
  this.current = 0;
  this.setValue(0);
  this.rate = dps/fps;
  var counter = this;
  var idle = (function () { counter.tick(); });
  this.timer = window.setInterval(idle, 1000/fps);
};

Counter.prototype.stop = function ()
{
  if (this.timer) {
    window.clearInterval(this.timer);
    this.timer = null;
  }
};

Counter.prototype.tick = function ()
{
  if (this.active && this.focused) {
    this.current += 1;
    this.setValue(this.current*this.rate);
  }
};

Counter.prototype.setDigit = function (i, v)
{
  var v0 = Math.floor(v);
  var d0 = this.digits0[i];
  var d1 = this.digits1[i];
  var h0 = d0.getBoundingClientRect().height;
  var dy = Math.floor(h0*(v0-v));
  d0.innerHTML = (v0 % this.base).toString(this.base);
  d0.style.top = (dy)+"px";
  d1.innerHTML = ((v0+1) % this.base).toString(this.base);
  d1.style.top = (dy+h0)+"px";
};

Counter.prototype.setValue = function (v)
{
  if (this.decimal) {
    this.decimal.innerHTML = Math.floor(v+0.1);
  }
  for (var i = 0; i < this.digits0.length; i++) {
    var vb = Math.floor(v/this.base);
    var vd = v-vb*this.base;
    this.setDigit(i, vd);
    if (Math.floor(vd) == this.base-1) {
      v = vb+(v-Math.floor(v));
    } else {
      v = vb;
    }
  }
};

Counter.prototype.setBase = function (v)
{
  this.stop();
  this.clean();
  switch (v) {
  case "16":
    this.init(16, 8, 20, 40);
    this.start(15, 2);
    break;
  case "10":
    this.init(10, 8, 20, 40);
    this.start(15, 1);
    break;
  case "8":
    this.init(8, 8, 20, 40);
    this.start(15, 1);
    break;
  case "2":
    this.init(2, 8, 20, 40);
    this.start(15, 0.5);
    break;
  }
};

Counter.prototype.setBaseSelector = function (obj)
{
  var counter = this;
  function change(e) { counter.setBase(obj.value); }
  obj.addEventListener("change", change);
  change();
};

Counter.prototype.setRunningBox = function (obj)
{
  var counter = this;
  function change(e) { counter.active = obj.checked; }
  obj.addEventListener("change", change);
  change();
};

Counter.prototype.setVisibleBox = function (obj)
{
  var counter = this;
  function change(e) { counter.decimal.hidden = !obj.checked; }
  obj.addEventListener("change", change);  
  change();
};
