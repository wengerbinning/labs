<script setup>

</script>



<template>
    <div>
        <ul>
            <li v-for="(each, idex) in type" :key="each.id">
                {{ each.id }} - {{ each.name }}
            </li>
        </ul>
        <p>内容</p>
        <ul>
            <li v-for="(each, idex) in data" :key="each.name">
                {{ each.name }} - {{ each.type }} {{ each.attrs }}
            </li>
        </ul>
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