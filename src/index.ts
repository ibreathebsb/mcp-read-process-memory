import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import { z } from "zod";
import { execFile } from "child_process";

// Create server instance
const server = new McpServer({
  name: "read-process-memory",
  version: "1.0.0",
  capabilities: {
    resources: {},
    tools: {},
  },
});

server.tool(
  "read-process-memory",
  "Read memory of specified process",
  {
    address: z
      .string()
      .regex(/^0[xX][0-9a-fA-F]+$/)
      .describe("The target memory address in HEXADECIMAL format"),
    type: z
      .enum(["uint8", "uint16", "uint32", "uint64", "bytes", "utf16"])
      .describe("The type of memory to read"),
    processId: z
      .number()
      .positive()
      .describe("The ID of the process to read memory from"),
    size: z
      .number()
      .positive()
      .optional()
      .describe("The number of bytes to read (only for type 'bytes')"),
  },
  async ({ address, type, processId, size }) => {
    const actualAddress = parseInt(address, 0);
    if (isNaN(actualAddress)) {
      return {
        content: [
          {
            type: "text",
            text: "address must be in HEXADECIMAL format with prefix 0x or 0X",
          },
        ],
      };
    }

    const payload: {
      address: number;
      type: string;
      processId: number;
      size?: number;
    } = { address: actualAddress, type, processId };

    if (type === "bytes") {
      if (!size || size <= 0) {
        return {
          content: [
            {
              type: "text",
              text: "A positive 'size' is required for type 'bytes'",
            },
          ],
        };
      }
      payload.size = size;
    }

    const data = await fetch(`http://127.0.0.1:20258/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(payload),
    });
    const res = await data.json();

    if (res && res.success) {
      let resultText: string;
      if (typeof res.data === 'number') {
        resultText = "0x" + res.data.toString(16);
      } else if (typeof res.data === 'string') {
        resultText = res.data;
      } else if (Array.isArray(res.data)) {
        resultText = JSON.stringify(res.data);
      } else {
        resultText = "failed to parse response data";
      }
      return {
        content: [
          {
            type: "text",
            text: resultText,
          },
        ],
      };
    }
    return {
      content: [
        {
          type: "text",
          text: "failed to read memory",
        },
      ],
    };
  }
);

async function main() {
  const transport = new StdioServerTransport();
  await server.connect(transport);
}

const pmm = execFile(__dirname + "/PMM.exe", (error, stdout, stderr) => {
  if (error) {
    console.error("Error starting PMM:", error);
    process.exit(1);
  }
});

process.on("exit", () => {
  pmm.kill();
});

main().catch((error) => {
  console.error("Fatal error in main():", error);
  process.exit(1);
});
