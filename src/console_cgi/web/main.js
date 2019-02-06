let jquery = require("jquery");
window.$ = window.jQuery = jquery;

require("jquery-ui-dist/jquery-ui.js");
import "jquery-ui-dist/jquery-ui.css";

$(function() {
  jQuery.fn.center = function() {
    this.css("position", "absolute");
    this.css(
      "top",
      Math.max(
        0,
        ($(window).height() - $(this).outerHeight()) / 2 + $(window).scrollTop()
      ) + "px"
    );
    this.css(
      "left",
      Math.max(
        0,
        ($(window).width() - $(this).outerWidth()) / 2 + $(window).scrollLeft()
      ) + "px"
    );
    return this;
  };
});

import Vue from "vue";
import App from "./app";

window.app = new Vue({
  el: "#app",
  render: h => h(App)
}).$children[0];

(() => {
  window.ct = (host, port, file) => window.app.createTerminal(host, port, file);
  window.ac = (tidx, cmd) => window.app.addCommand(tidx, cmd);
  window.gnc = tidx => window.app.gotoNextCommand(tidx);
  window.dop = (tidx, output) => window.app.displayOutput(tidx, output);
  window.derr = (tidx, err) => window.app.displayError(tidx, err);
})();
