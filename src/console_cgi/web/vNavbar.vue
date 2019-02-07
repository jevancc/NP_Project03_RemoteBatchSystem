<template>
  <nav>
    <ul>
      <li v-show="!terminal.display.closed" v-for="terminal in terminals">
        <a
          :class="{ highlight: !terminal.display.minimized }"
          @click="toggleMinimized(terminal)"
        >
          {{ terminal.host }}:{{ terminal.port }}<br />{{ terminal.file }}
        </a>
      </li>
    </ul>
  </nav>
</template>
<script>
export default {
  props: ["terminals"],
  methods: {
    toggleMinimized(terminal) {
      if (terminal.display.minimized) {
        terminal.display.minimized = false;
        this.$emit("focus-terminal", terminal);
      } else if (
        !this.terminals.some(
          t =>
            !t.display.closed &&
            !t.display.minimized &&
            t.display.style.z > terminal.display.style.z
        )
      ) {
        terminal.display.minimized = true;
      } else {
        this.$emit("focus-terminal", terminal);
      }
    }
  }
};
</script>
<style scoped>
nav {
  z-index: 999;
  position: fixed;
  bottom: 0;
  left: 0;
  width: 100%;
  margin: 0;
}

nav ul {
  background: rgba(255, 255, 255, 0.1);
  width: 100%;
  margin: 0;
}

nav ul li {
  display: inline-block;
  text-align: center;
  margin: 0px 2.5px;
}

nav ul li a {
  padding: 2px 6px;
  font-family: monospace;
  color: rgba(255, 255, 255, 0.6);
  font-size: 1.2em;
  text-decoration: none;
  display: block;
}

nav ul li a:hover {
  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
  color: rgba(255, 255, 255, 0.9);
  background: rgba(255, 255, 255, 0.1);
}

nav ul li a.highlight {
  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
  color: rgba(255, 255, 255, 0.9);
  background: rgba(255, 255, 255, 0.2);
}

nav ul li a.highlight:hover {
  background: rgba(255, 255, 255, 0.3);
}
</style>
