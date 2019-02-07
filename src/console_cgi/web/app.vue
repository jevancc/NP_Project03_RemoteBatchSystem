<template>
  <div class="app-background">
    <v-terminal
      v-for="(terminal, i) in terminals"
      :terminal="terminal"
      v-on:focus-terminal="onFocusTerminal"
    >
    </v-terminal>
    <v-navbar
      :terminals="terminals"
      v-on:focus-terminal="onFocusTerminal"
    ></v-navbar>
  </div>
</template>

<script>
import vTerminal from "./vTerminal";
import vNavbar from "./vNavbar";

export default {
  components: {
    vTerminal,
    vNavbar
  },
  data() {
    return {
      terminals: []
    };
  },
  methods: {
    createTerminal(host, port, file) {
      this.terminals.push({
        host,
        port,
        file,
        currentCommandIndex: 0,
        data: [
          {
            command: null,
            output: "",
            error: null
          }
        ],
        display: {
          minimized: false,
          closed: false,
          style: {
            w: 600,
            h: 425,
            x: this.terminals.length * 30 + 20,
            y: this.terminals.length * 100 + 20,
            z: this.terminals.length
          }
        }
      });
    },
    addCommand(tidx, cmd = "") {
      this.terminals[tidx].data.push({
        command: cmd,
        output: "",
        error: null
      });
    },
    gotoNextCommand(tidx) {
      let i = this.terminals[tidx].currentCommandIndex;
      if (i < this.terminals[tidx].data.length) {
        this.terminals[tidx].currentCommandIndex += 1;
      }
    },
    displayOutput(tidx, output = "") {
      let i = this.terminals[tidx].currentCommandIndex;
      this.terminals[tidx].data[i].output += output;
    },
    displayError(tidx, err = null) {
      let i = this.terminals[tidx].currentCommandIndex;
      this.terminals[tidx].data[i].error = err;
    },
    onFocusTerminal(terminal) {
      let newZIndex = this.terminals.length;
      this.terminals.forEach(t => {
        if (t.display.style.z > terminal.display.style.z) {
          t.display.style.z--;
        }
      });
      terminal.display.style.z = newZIndex;
    }
  }
};
</script>
<style>
.app-background {
  height: 100%;
  width: 100%;
}

::-webkit-scrollbar-corner {
  background: rgba(0, 0, 0, 0);
}

.vdr {
  border-width: 0px !important;
}

.handle {
  background-color: transparent !important;
  border-width: 0px !important;
}
.handle.handle-tm {
  width: 100% !important;
  left: 5px !important;
}
.handle.handle-bm {
  width: 100% !important;
  left: 5px !important;
}
.handle.handle-mr {
  height: 100% !important;
  top: 5px !important;
}
.handle.handle-ml {
  height: 100% !important;
  top: 5px !important;
}
.handle.handle-tm {
  width: 100% !important;
  left: 5px !important;
}
.handle.handle-bm {
  width: 100% !important;
  left: 5px !important;
}
.handle.handle-mr {
  height: 100% !important;
  top: 5px !important;
}
.handle.handle-ml {
  height: 100% !important;
  top: 5px !important;
}
</style>
