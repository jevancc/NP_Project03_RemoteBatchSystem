<template>
  <div
    class="window"
    ref="window"
    :style="{ 'z-index': terminal.display.zIndex }"
    v-show="!terminal.display.closed && !terminal.display.minimized"
    @mousedown="$emit('focus-terminal', terminal)"
  >
    <div class="window-menu" ref="window-menu">
      <div class="window-button window-button-close" @click="close()"></div>
      <div
        class="window-button window-button-minimize"
        @click="minimize()"
      ></div>
      <div class="window-button window-button-zoom" @click="zoom()"></div>
      <div class="window-title">
        {{ terminal.host }}:{{ terminal.port }} - {{ terminal.file }}
      </div>
    </div>
    <div class="window-screen scroll">
      <span v-for="(line, i) in terminal.data">
        <pre
          v-if="line.command !== null"
          :class="[
            i <= terminal.currentCommandIndex
              ? 'prompt-text'
              : 'unexecuted-prompt-text'
          ]"
        >
%&nbsp;{{ line.command }}</pre
        >
        <pre v-if="line.output">{{ line.output }}</pre>
        <pre class="error-text" v-if="line.error">{{ line.error }}</pre>
      </span>
    </div>
  </div>
</template>
<script>
export default {
  props: ["terminal", "initialPosition"],
  methods: {
    minimize() {
      this.terminal.display.minimized = true;
    },
    zoom() {
      $(this.$refs.window)
        .height($(window).height() - 50)
        .width($(window).width() - 50)
        .center();
    },
    close() {
      this.terminal.display.closed = true;
    }
  },
  mounted() {
    $(this.$refs.window)
      .offset(this.initialPosition)
      .draggable({
        handle: ".window-menu"
      })
      .resizable({
        minWidth: 400,
        minHeight: 200,
        handles: "n, e, s, w, se, ne, sw, nw"
      });
  }
};
</script>
<style scoped>
.window {
  width: 600px;
  height: 425px;
  margin: 0px;
  padding-bottom: 25px;
  position: absolute;
  box-shadow: 0 0 8px 1px #666666;
  border-radius: 5px;
}

.window-button {
  height: 10px;
  width: 10px;
  border-radius: 50%;
  border: 1px solid #000;
  position: relative;
  top: 6px;
  left: 6px;
  background-color: #ff3b47;
  border-color: #9d252b;
  display: inline-block;
}

.window-button-minimize {
  left: 11px;
  background-color: #ffc100;
  border-color: #9d802c;
}

.window-button-zoom {
  left: 16px;
  background-color: #00d742;
  border-color: #049931;
}

.window-title {
  top: 4px;
  left: 68px;
  right: 7px;
  text-align: center;
  font-size: 1.2em;
  font-family: monospace;
  position: absolute;

  text-overflow: ellipsis;
  white-space: nowrap;
  overflow: hidden;
}

.window-menu {
  position: absolute;
  right: 0;
  left: 0;
  box-sizing: border-box;
  height: 25px;
  background-color: #bbb;
  border-top-right-radius: 5px;
  border-top-left-radius: 5px;
}

.window-screen {
  position: relative;
  background-color: #151515;
  box-sizing: border-box;
  top: 25px;
  width: 100%;
  height: 100%;
  padding: 5px 20px;
  border-bottom-left-radius: 5px;
  border-bottom-right-radius: 5px;
  overflow-y: scroll;
}

pre {
  position: relative;
  text-align: left;
  font-size: 1.2em;
  font-family: monospace;
  word-break: break-all;

  width: 100%;
  color: #ffffff;
  margin: 5px 0px;
}

.prompt-text {
  color: rgba(240, 255, 64, 1);
}

.unexecuted-prompt-text {
  color: rgba(240, 255, 64, 0.4);
}

.error-text {
  color: red;
}

span {
  color: #fff;
  font-weight: bold;
}

.scroll::-webkit-scrollbar-track {
  -webkit-box-shadow: inset 0 0 6px rgba(0, 0, 0, 0.3);
  border-radius: 5px;
}

.scroll::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}

.scroll::-webkit-scrollbar-thumb {
  border-radius: 5px;
  -webkit-box-shadow: inset 0 0 6px rgba(0, 0, 0, 0.3);
  background-color: #555;
}
</style>
