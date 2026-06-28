import type { RPCSchema } from "electrobun";

export type MainRPC = RPCSchema<{
  requests: {
    createEngine: {
      params: Record<string, never>;
      response: number;
    };

    sendEngineCommand: {
      params: {
        id: number;
        command: string;
      };
      response: void;
    };

    releaseEngine: {
      params: {
        id: number;
      };
      response: void;
    };
  };

  messages: {
    engineAnswer: {
      id: number;
      line: string;
    };
  };
}>;

export type WebviewRPC = RPCSchema<{
  requests: {
    createEngine: {
      params: Record<string, never>;
      response: number;
    };

    sendEngineCommand: {
      params: {
        id: number;
        command: string;
      };
      response: void;
    };
  };

  messages: {
    engineAnswer: {
      params: {
        engineId: number;
        line: string;
      };
    };
  };
}>;
