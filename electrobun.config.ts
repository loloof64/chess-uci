import type { ElectrobunConfig } from "electrobun/bun";

export default {
  app: {
    name: "chess-uci",
    identifier: "com.loloof64.chess_uci",
    version: "0.1.0",
  },
  build: {
    useAsar: true,
    bun: {
      entrypoint: "src/bun/index.ts",
      external: [],
    },
    views: {},
    copy: {
      "dist/": "views/mainview/",
    },
    watch: ["dist/**", "src/**"],
    mac: {
      codesign: false,
      notarize: false,
      bundleCEF: false,
      entitlements: {},
    },
    linux: {
      bundleCEF: false,
    },
    win: {
      bundleCEF: false,
    },
  },
  release: {
    baseUrl: "",
  },
} satisfies ElectrobunConfig;
