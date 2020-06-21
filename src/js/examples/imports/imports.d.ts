// --- Simple imports
import IsOk from "./import_source";
import { str } from "./import_source";
import * as Others from "./import_source";

type should_be_bool = IsOk;
type should_be_string = str;
type should_be_obj = Others.iImport;
