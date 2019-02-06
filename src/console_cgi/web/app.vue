<template>
  <div>
    <v-terminal
      v-for="(terminal, i) in terminals"
      :terminal="terminal"
      :initialPosition="{ top: i * 30 + 20, left: i * 100 + 20 }"
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
          zIndex: this.terminals.length
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
        if (t.display.zIndex > terminal.display.zIndex) {
          t.display.zIndex--;
        }
      });
      terminal.display.zIndex = newZIndex;
    }
  }
};
</script>
<style></style>
