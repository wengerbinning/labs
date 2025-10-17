<script setup>
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
import CallGraph  from './components/CallGraph.vue'
</script>

<template>
    <HelloWorld />

</template>

<style scoped>
html, body {
  margin: 0;
  padding: 0;
  height: 100%;
}

#app {
  position: fixed;   /* 固定在视口 */
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
}
</style>
