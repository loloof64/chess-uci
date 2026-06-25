import { electrobun } from "@/lib/electrobun";
import { useEffect, useState } from "react";
import { EnginesZone } from "./mainscreen/engineszone";
import { MainToolbar } from "./mainscreen/maintoolbar";

export function App() {
	const addEngine =
        (id:number) =>
        {
            console.log(
                "New engine",
                id
            );
        };

	return (
		<div className="min-h-screen flex flex-col items-center justify-start gap-6 p-8 bg-linear-to-tr from-orange-600 to-blue-800">
			<MainToolbar addEngineCb={addEngine} />
			<EnginesZone />
		</div>
	);
}
