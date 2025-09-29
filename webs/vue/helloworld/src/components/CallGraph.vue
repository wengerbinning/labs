<script setup>

</script>



<template>
    <div id="list">
        <div v-for="(each, idex) in data" :key="each.name">
            <p> {{ each.name }} </p>
        </div>
    </div>
</template>


<script>
export default {
    data() {
        return {
            type: [
                { id: "type",     name: "数据类型" },
                { id: "object",   name: "数据对象" },
                { id: "function", name: "函数接口" }
            ],
            data: [
                {
                    name: "int",
                    type: "type",
                    size: 4,
                    opts: [],
                    apis: [],

                    attrs: ["standard"],
                    scope: [],
                    label: [],
                    state: [],
                },
                {
                    name: "var",
                    type: "object",
                    size: 1,
                    opts: [],
                    apis: [],

                    attrs: ["standard"],
                    scope: [],
                    label: [],
                    state: [],
                },
                {
                    name: "func",
                    type: "function",
                    size: 1,
                    opts: [],
                    apis: [],

                    attrs: ["standard"],
                    scope: [],
                    label: [],
                    state: [],

                    params: [],
                    return: []

                }
            ]
        }
    }
}
</script>

<style scoped>
#list {
    width: 100%;
    height: 100%;
    border: 1px solid black;
}
</style>