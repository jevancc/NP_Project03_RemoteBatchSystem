import Vue from "vue";
import App from "./app";

import VueDraggableResizable from "vue-draggable-resizable";
// optionally import default styles
import "vue-draggable-resizable/dist/VueDraggableResizable.css";
Vue.component("vue-draggable-resizable", VueDraggableResizable);

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
