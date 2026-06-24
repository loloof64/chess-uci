export type EngineId =
    number;



export type EngineOutputCallback =
    (
        line:string
    ) => void;



export interface EngineOptions
{
    debug?: boolean;
}



export interface GoOptions
{
    movetime?: number;

    depth?: number;

    infinite?: boolean;
}



export interface EngineInfo
{
    id:EngineId;

    ready:boolean;

    searching:boolean;
}